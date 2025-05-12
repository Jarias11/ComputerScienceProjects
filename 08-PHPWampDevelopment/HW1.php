<?php
$servername = "localhost";
$username = "root";
$password = "password";
//Create connection 

$conn = new mysqli($servername, $username, $password);
//Check connection 
if($conn->connect_error){
	die("Connection Failed: ".$conn->connect_error);
}
/*
$db = "CREATE DATABASE student20";
if($conn->query($db) === TRUE){
    echo "DATABASE Created Successfully!\n";
    
}else{
    echo"Error";
}
$dbName = "student20";
$conn = new mysqli($servername, $username, $password,$dbName);
//Create table
$tb = "	CREATE TABLE student1 (
	id INTEGER UNSIGNED AUTO_INCREMENT PRIMARY KEY,
	StudentName varchar(30) NOT NULL,
	Grade INTEGER NOT NULL)";
if ($conn->query($tb) === TRUE) {
    echo "TABLE created successfully";
} else {
    echo "Error creating TABLE: " . $conn->error;
}

*/
$dbName = "student20";
$conn = new mysqli($servername, $username, $password,$dbName);
$fetch = "SELECT *
	FROM student1
	WHERE Grade>=80
	";
$result = $conn->query($fetch);
if ($result->num_rows> 0){
	while($row = $result->fetch_assoc()){
	echo "id: " . $row["id"]. " - Name: " . $row["StudentName"]. " " . $row["Grade"]. "<br>";	
}
}

$conn->close();
?>