<?php
// Set the content type and character encoding
header("Content-Type: text/html; charset=utf-8"); ?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>CGI Test Result</title>
    <link
        href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;700&display=swap"
        rel="stylesheet"
    />
    <link
        href="https://fonts.googleapis.com/css2?family=VT323&display=swap"
        rel="stylesheet"
    />
    <style>
        :root {
            --primary-color: #007bff; /* Blue */
            --primary-hover-color: #0056b3;
            --success-color: #28a745; /* Green for success */
            --error-color: #dc3545; /* Red for error */
            --background-color: #e9eff6;
            --card-background: #ffffff;
            --text-color: #343a40;
            --border-color: #dee2e6;
            --shadow-light: 0 4px 8px rgba(0, 0, 0, 0.05);
            --shadow-medium: 0 8px 16px rgba(0, 0, 0, 0.1);

            /* Terminal colors */
            --terminal-glow-color: #00ff41;
            --terminal-bg-color: #0a0a0a;
        }

        body {
            font-family: "Roboto", sans-serif;
            margin: 0;
            padding: 40px 20px;
            background-color: var(--background-color);
            color: var(--text-color);
            display: flex;
            flex-direction: column;
            align-items: center;
            min-height: 100vh;
            box-sizing: border-box;
        }

        h1 {
            color: #2c3e50;
            font-weight: 700;
            margin-bottom: 40px;
            text-align: center;
            font-size: 2.5em;
        }

        .container {
            display: flex;
            flex-direction: column;
            gap: 30px;
            max-width: 800px;
            width: 100%;
        }

        .test-section {
            background-color: var(--card-background);
            padding: 30px;
            border-radius: 12px;
            box-shadow: var(--shadow-medium);
            border: 1px solid var(--border-color);
        }

        h2 {
            color: var(--primary-color);
            border-bottom: 2px solid var(--primary-color);
            padding-bottom: 10px;
            margin-top: 0;
            margin-bottom: 25px;
            font-weight: 600;
            font-size: 1.8em;
        }

        a {
            display: inline-block;
            background-color: var(--primary-color);
            color: white;
            padding: 12px 25px;
            border-radius: 6px;
            text-decoration: none;
            font-weight: 500;
            transition:
                background-color 0.3s ease,
                transform 0.2s ease,
                box-shadow 0.3s ease;
            box-shadow: var(--shadow-light);
            margin-top: 25px; /* Added margin for spacing */
        }

        a:hover {
            background-color: var(--primary-hover-color);
            transform: translateY(-2px);
            box-shadow: var(--shadow-medium);
        }

        /* --- Old Terminal Style --- */
        pre {
            background-color: var(--terminal-bg-color);
            color: var(--terminal-glow-color);
            font-family: 'VT323', monospace;
            font-size: 18px;
            line-height: 1.3;
            text-shadow: 0 0 5px var(--terminal-glow-color);

            padding: 20px;
            border: 0px solid var(--terminal-glow-color);
            box-shadow: 0 0 15px rgba(0, 255, 65, 0.4);
            border-radius: 0; /* Sharp corners */

            position: relative; /* For scanline pseudo-element */
            overflow-x: auto;
            white-space: pre-wrap;
            word-wrap: break-word;
        }

        /* Scanline Overlay Effect */
        pre::before {
            content: ' ';
            display: block;
            position: absolute;
            top: 0;
            left: 0;
            bottom: 0;
            right: 0;
            background: repeating-linear-gradient(
                0deg,
                rgba(0, 0, 0, 0.2),
                rgba(0, 0, 0, 0.5) 1px,
                transparent 1px,
                transparent 4px
            );
            pointer-events: none; /* Allows text selection underneath */
            z-index: 1;
        }

        /* Ensure code content is above the overlay */
        pre > code {
            position: relative;
            z-index: 2;
        }
        /* --- End Terminal Style --- */

        /* Responsive adjustments */
        @media (max-width: 768px) {
            h1 {
                font-size: 2em;
                margin-bottom: 30px;
            }
            .container {
                padding: 0 10px;
            }
            .test-section {
                padding: 25px;
            }
            h2 {
                font-size: 1.5em;
            }
            a {
                width: 100%;
                text-align: center;
            }
        }

        @media (max-width: 480px) {
            body {
                padding: 20px 10px;
            }
            h1 {
                font-size: 1.8em;
            }
            .test-section {
                padding: 20px;
            }
        }
    </style>
</head>
<body>
    <h1>CGI Test Result</h1>

    <div class="container">
        <div class="test-section">
            <h2>Query Parameters Received</h2>
            <pre><code><?php // Use htmlspecialchars to prevent XSS and print_r with 'true' to return a string

if (!empty($_GET)) {
                echo htmlspecialchars(print_r($_GET, true));
            } else {
                echo "No GET parameters were received.";
            } ?></code></pre>

            <h2>Server Environment Variables</h2>
            <pre><code><?php echo htmlspecialchars(
                print_r($_SERVER, true)
            ); ?></code></pre>

            <a href="index.html">Back to Tests</a>
        </div>
    </div>
</body>
</html>
