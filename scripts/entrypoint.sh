#!/bin/bash
set -e

# Function to handle cleanup on exit
cleanup() {
  echo "Shutting down webserv..."
  if pgrep webserv > /dev/null; then
    kill -SIGINT $(pidof webserv) 2>/dev/null || true
  fi
  exit 0
}

# Register the cleanup function for these signals
trap cleanup SIGTERM SIGINT

# Function to build the application with retry logic
build_with_retry() {
  local max_attempts=3
  local attempt=1
  local success=false

  while [ $attempt -le $max_attempts ] && [ "$success" = false ]; do
    echo "Build attempt $attempt of $max_attempts..."
    
    if make -C /app; then
      echo "Build successful!"
      success=true
    else
      echo "Build failed on attempt $attempt."
      if [ $attempt -lt $max_attempts ]; then
        echo "Retrying in 3 seconds..."
        sleep 3
      else
        echo "Maximum build attempts reached. Please fix the build errors."
        return 1
      fi
    fi
    
    ((attempt++))
  done
  
  return 0
}

# Function to start or restart the server
start_server() {
  echo "Starting webserv..."
  make run -C /app &
  SERVER_PID=$!
  echo "Webserv started with PID: $SERVER_PID"
}

# Initial build if needed
if [ ! -f "/app/build/webserv" ]; then
  echo "Initial build of webserv..."
  if ! build_with_retry; then
    echo "Initial build failed after multiple attempts. Waiting for file changes..."
  else
    start_server
  fi
else
  start_server
fi

# Watch for changes and rebuild/restart when detected
echo "Watching for changes in src/ and include/ directories..."
while true; do
  inotifywait -r -e modify,create,delete /app/src/ /app/include/
  echo "Change detected, rebuilding..."
  
  if pgrep webserv > /dev/null; then
    echo "Stopping current webserv instance..."
    kill -SIGINT $(pidof webserv) 2>/dev/null || true
  fi
  
  # Wait a moment to ensure the process has terminated
  sleep 2
  
  # Rebuild the application with retry logic
  if build_with_retry; then
    start_server
    echo "Webserv restarted successfully!"
  else
    echo "Build failed after multiple attempts. Waiting for new changes..."
  fi
done 