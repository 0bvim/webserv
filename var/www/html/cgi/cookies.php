<?php
// Set cookie
if ($_SERVER["REQUEST_METHOD"] === "POST" && isset($_POST["set_cookie"])) {
    $name = trim($_POST["cookie_name"] ?? "");
    $value = trim($_POST["cookie_value"] ?? "");
    if ($name && $value) {
        // Updated setcookie with Secure, HttpOnly, and SameSite=Lax for better security
        setcookie($name, $value, [
            "expires" => time() + 3600,
            "path" => "/",
            "secure" => isset($_SERVER["HTTPS"]), // Set secure if HTTPS is detected
            "httponly" => true,
            "samesite" => "Lax",
        ]);
        header("Location: " . $_SERVER["PHP_SELF"]);
        exit();
    }
}

// Delete last cookie
if (
    $_SERVER["REQUEST_METHOD"] === "POST" &&
    isset($_POST["delete_last_cookie"])
) {
    if (!empty($_COOKIE)) {
        $lastCookieName = array_key_last($_COOKIE);
        // Updated setcookie for deletion with Secure, HttpOnly, and SameSite=LLax
        setcookie($lastCookieName, "", [
            "expires" => time() - 3600, // Past expiration time to delete
            "path" => "/",
            "secure" => isset($_SERVER["HTTPS"]),
            "httponly" => true,
            "samesite" => "Lax",
        ]);
    }
    header("Location: " . $_SERVER["PHP_SELF"]);
    exit();
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Advanced Cookie Tester</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;700&display=swap" rel="stylesheet">
    <style>
        :root {
            --primary-color: #4CAF50; /* Green */
            --primary-hover-color: #45a049;
            --danger-color: #f44336; /* Red */
            --danger-hover-color: #da190b;
            --background-color: #f7f9fc;
            --card-background: #ffffff;
            --text-color: #333;
            --border-color: #ddd;
            --shadow-light: 0 4px 8px rgba(0,0,0,0.05);
            --shadow-medium: 0 6px 12px rgba(0,0,0,0.1);
        }

        body {
            font-family: 'Roboto', sans-serif;
            margin: 0;
            padding: 40px 20px;
            background-color: var(--background-color);
            color: var(--text-color);
            display: flex;
            justify-content: center;
            align-items: flex-start;
            min-height: 100vh;
        }

        .container {
            background-color: var(--card-background);
            padding: 30px;
            border-radius: 12px;
            box-shadow: var(--shadow-medium);
            max-width: 600px;
            width: 100%;
            box-sizing: border-box;
        }

        h2, h3 {
            color: #2c3e50;
            border-bottom: 2px solid var(--primary-color);
            padding-bottom: 10px;
            margin-top: 30px;
            font-weight: 700;
        }

        h2:first-of-type {
            margin-top: 0;
        }

        form {
            margin-bottom: 20px;
        }

        .form-group {
            margin-bottom: 15px;
            display: flex;
            align-items: center;
        }

        label {
            display: inline-block;
            width: 120px; /* Increased width for better alignment */
            font-weight: 400;
            color: #555;
            margin-right: 10px;
        }

        input[type="text"] {
            flex-grow: 1;
            padding: 12px 15px;
            border: 1px solid var(--border-color);
            border-radius: 6px;
            font-size: 16px;
            box-sizing: border-box;
            transition: border-color 0.3s ease, box-shadow 0.3s ease;
        }

        input[type="text"]:focus {
            border-color: var(--primary-color);
            box-shadow: 0 0 0 3px rgba(76, 175, 80, 0.2);
            outline: none;
        }

        button {
            padding: 12px 25px;
            border: none;
            border-radius: 6px;
            font-size: 16px;
            cursor: pointer;
            transition: background-color 0.3s ease, transform 0.2s ease, box-shadow 0.3s ease;
            font-weight: 500;
            margin-top: 10px; /* Added margin for buttons */
        }

        button.primary {
            background-color: var(--primary-color);
            color: white;
            box-shadow: var(--shadow-light);
        }

        button.primary:hover {
            background-color: var(--primary-hover-color);
            transform: translateY(-2px);
            box-shadow: var(--shadow-medium);
        }

        button.danger {
            background-color: var(--danger-color);
            color: white;
            box-shadow: var(--shadow-light);
        }

        button.danger:hover {
            background-color: var(--danger-hover-color);
            transform: translateY(-2px);
            box-shadow: var(--shadow-medium);
        }

        .cookie-list ul {
            list-style: none;
            padding: 0;
            border: 1px solid var(--border-color);
            border-radius: 8px;
            background-color: #fdfdfd;
            box-shadow: var(--shadow-light);
        }

        .cookie-list li {
            padding: 12px 15px;
            border-bottom: 1px dashed var(--border-color);
            display: flex;
            justify-content: space-between;
            align-items: center;
            font-size: 15px;
        }

        .cookie-list li:last-child {
            border-bottom: none;
        }

        .cookie-list li strong {
            color: #2c3e50;
            flex-shrink: 0; /* Prevent strong from shrinking */
            margin-right: 10px; /* Space between name and value */
        }

        .cookie-list p {
            font-style: italic;
            color: #777;
            text-align: center;
            margin-top: 20px;
            padding: 10px;
            background-color: #e9ecef;
            border-radius: 8px;
            border: 1px solid #dee2e6;
        }

        /* Responsive adjustments */
        @media (max-width: 600px) {
            body {
                padding: 20px 10px;
            }
            .form-group {
                flex-direction: column;
                align-items: flex-start;
            }
            label {
                width: auto;
                margin-bottom: 5px;
            }
            input[type="text"] {
                width: 100%;
            }
            button {
                width: 100%;
                margin-top: 15px;
            }
            form {
                display: block !important; /* Override inline display for delete form */
            }
        }
    </style>
</head>
<body>

    <div class="container">
        <h2>Set a Cookie</h2>
        <form method="POST">
            <div class="form-group">
                <label for="cookie_name">Name:</label>
                <input type="text" name="cookie_name" id="cookie_name" placeholder="e.g., username" required>
            </div>
            <div class="form-group">
                <label for="cookie_value">Value:</label>
                <input type="text" name="cookie_value" id="cookie_value" placeholder="e.g., JohnDoe" required>
            </div>
            <button type="submit" name="set_cookie" class="primary">Set Cookie</button>
        </form>


        <h2>Actions</h2>
        <form method="POST" style="display: inline;">
            <button type="submit" name="delete_last_cookie" class="danger">Delete Last Cookie</button>
        </form>

        <div class="cookie-list">
            <h3>Current Cookies</h3>
            <?php if (!empty($_COOKIE)) {
                echo "<ul>";
                foreach ($_COOKIE as $name => $value) {
                    echo "<li><strong>" .
                        htmlspecialchars($name) .
                        ":</strong> " .
                        htmlspecialchars($value) .
                        "</li>";
                }
                echo "</ul>";
            } else {
                echo "<p>No cookies set for this domain.</p>";
            } ?>
        </div>
    </div>

</body>
</html>
