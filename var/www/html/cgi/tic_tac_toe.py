#!/usr/bin/env python3
import cgi
import cgitb
cgitb.enable()

print("Content-Type: text/html\n")
print("""<!DOCTYPE html>
<html>
<head>
    <title>Tic Tac Toe</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background: #f9f9f9;
            text-align: center;
            padding: 30px;
        }
        h1 {
            color: #333;
        }
        table {
            border-collapse: collapse;
            margin: 20px auto;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
        }
        td {
            width: 80px;
            height: 80px;
            border: 2px solid #555;
            text-align: center;
            vertical-align: middle;
            font-size: 32px;
            font-weight: bold;
            background-color: #fff;
        }
        button {
            width: 100%;
            height: 100%;
            border: none;
            background: #f0f0f0;
            font-size: 28px;
            font-weight: bold;
            color: #444;
            cursor: pointer;
        }
        button:hover {
            background: #e0e0e0;
        }
        .message {
            font-size: 22px;
            color: #222;
            margin: 20px;
        }
        .restart-form {
            margin-top: 20px;
        }
        .restart-button {
            font-size: 16px;
            padding: 10px 20px;
            background: #007BFF;
            color: white;
            border: none;
            cursor: pointer;
            border-radius: 5px;
        }
        .restart-button:hover {
            background: #0056b3;
        }
    </style>
</head>
<body>
    <h1>Tic Tac Toe</h1>""")

form = cgi.FieldStorage()

# Initialize or get game state
current_board = form.getvalue('board', ',,,,,,,,').split(',')
current_player = form.getvalue('current_player', 'X')

# Check for restart
if 'restart' in form:
    current_board = [''] * 9
    current_player = 'X'
else:
    # Process move if available
    move = form.getvalue('move')
    if move is not None and move.isdigit():
        move = int(move)
        if 0 <= move < 9 and current_board[move] == '':
            current_board[move] = current_player
            current_player = 'O' if current_player == 'X' else 'X'

# Check for winner
def check_winner(board):
    lines = [
        [0, 1, 2], [3, 4, 5], [6, 7, 8],  # Rows
        [0, 3, 6], [1, 4, 7], [2, 5, 8],  # Columns
        [0, 4, 8], [2, 4, 6]             # Diagonals
    ]
    for a, b, c in lines:
        if board[a] and board[a] == board[b] == board[c]:
            return board[a]
    return None

winner = check_winner(current_board)
tie = not winner and '' not in current_board

# Display game status
if winner:
    print(f'<div class="message">Player {winner} wins!</div>')
elif tie:
    print('<div class="message">Game is a tie!</div>')
else:
    print(f'<div class="message">Current Player: {current_player}</div>')

# Generate game board
print('<form method="POST">')
print(f'<input type="hidden" name="board" value="{",".join(current_board)}">')
print(f'<input type="hidden" name="current_player" value="{current_player}">')
print('<table>')

for row in range(3):
    print('<tr>')
    for col in range(3):
        cell = row * 3 + col
        value = current_board[cell]
        if value == '' and not winner and not tie:
            print(f'<td><button type="submit" name="move" value="{cell}">&nbsp;</button></td>')
        else:
            display = value if value else '&nbsp;'
            print(f'<td>{display}</td>')
    print('</tr>')

print('</table>')
print('</form>')

# Restart Button
print('''
<form method="POST" class="restart-form">
    <input type="hidden" name="restart" value="1">
    <button type="submit" class="restart-button">Restart Game</button>
</form>
''')

print("</body></html>")
