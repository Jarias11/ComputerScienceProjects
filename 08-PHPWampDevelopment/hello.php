<?php
$heads = 0;
$tails = 0;
for($count = 1; $count<= 100; $count++){
$x = rand(1,2);
if($x == 1)
	$heads++;
	else
	$tails++;

}
echo"<p>heads: {$heads}</p>";
echo"<p>tails: {$tails}</p>";
?>

heads: 48

tails: 52