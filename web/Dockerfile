# Use the official Nginx base image
FROM nginx:latest

# Copy custom configuration file from the current directory
# to the Nginx configuration directory in the container
COPY nginx.conf /etc/nginx/nginx.conf

# Copy static website files to the default Nginx HTML directory
COPY . /usr/share/nginx/html/

# Expose port 80 to the host
EXPOSE 8040

# Start Nginx when the container starts
CMD ["nginx", "-g", "daemon off;"]
