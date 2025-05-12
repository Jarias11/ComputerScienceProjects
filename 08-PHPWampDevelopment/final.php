<?php
$servername = "localhost";
$username = "root";
$password = "password";

/*
$conn = new mysqli($servername, $username, $password);
//Check connection 
if($conn->connect_error){
	die("Connection Failed: ".$conn->connect_error);
}

$db = "CREATE DATABASE sales1";
if($conn->query($db) === TRUE){
    echo "DATABASE Created Successfully!\n";
    
}else{
    echo"Error";
}
*/
$dbName = "sales1";
$conn = new mysqli($servername, $username, $password,$dbName);

/*
//Create table
$tb = "	CREATE TABLE sales2(
	id INTEGER UNSIGNED AUTO_INCREMENT PRIMARY KEY,
	Emp_Name varchar(30) NOT NULL,
	Sales INTEGER NOT NULL)";
if ($conn->query($tb) === TRUE) {
    echo "TABLE created successfully";
} else {
    echo "Error creating TABLE: " . $conn->error;
}

*/

$fetch = "SELECT *
	FROM sales2
	WHERE Sales>=900
	";
$result = $conn->query($fetch);
if ($result->num_rows> 0){
	while($row = $result->fetch_assoc()){
	echo "id: " . $row["id"]. " - Name: " . $row["Emp_Name"]. " $" . $row["Sales"]. "<br>";	
}
}
$conn->close();
?>