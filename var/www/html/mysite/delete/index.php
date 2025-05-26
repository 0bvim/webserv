<?php

$file = 'data.json';

if (file_exists($file))
{
	$json_str = file_get_contents($file);
	$data = json_decode($json_str, true);
	var_dump($data["users"]);
}
else
{
	echo "File not found";
}

// header("Location: /");