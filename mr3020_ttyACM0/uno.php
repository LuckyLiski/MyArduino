<?php  

if (isset($_GET['action'])) {  
    require("php_serial.class.php");  
    $serial = new phpSerial();  

    
    $serial->deviceSet("/dev/ttyACM0"); //Имя порта  
      
     
    $serial->confBaudRate(57600); //Скорость порта: 57600  
    $serial->deviceOpen();  

    
    if ($_GET['action'] == "SW_1_on") {  
        $serial->sendMessage("1\r");  
      
    } else if ($_GET['action'] == "SW_1_off") {  
       $serial->sendMessage("2\r");  
    } 
     
   if ($_GET['action'] == "SW_2_on") {  
        $serial->sendMessage("3\r");  
      
    } else if ($_GET['action'] == "SW_2_off") {  
       $serial->sendMessage("4\r");  
    } 
         
    if ($_GET['action'] == "SW_3_on") {  
        $serial->sendMessage("4\r");  
      
    } else if ($_GET['action'] == "SW_3_off") {  
       $serial->sendMessage("5\r");  
    } 
        
    if ($_GET['action'] == "SW_4_on") {  
        $serial->sendMessage("6\r");  
      
    } else if ($_GET['action'] == "SW_4_off") {  
       $serial->sendMessage("7\r");  
    } 
      
     $serial->deviceClose();  

}  


?><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"  
"http://www.w3.org/TR/html4/loose.dtd">  
<html>  
<head>  
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">  
<title>Serial Control</title>  
</head>  
<body>  
    <table> 
                 <tr> 
                   <td><p><a href="<?=$_SERVER['PHP_SELF'] . "?action=SW_1_on" ?>"> 
<button><img src="onoff.png" alt="onoff" style="vertical-align: middle"> SW1 ON </button></p></a></p>  
</td> 
                   <td><p><a href="<?=$_SERVER['PHP_SELF'] . "?action=SW_2_on" ?>"> 
<button><img src="onoff.png" alt="onoff" style="vertical-align: middle"> SW2 ON </button></p></a></p>  
     
</td> 
<!--
                   <td><p><a href="<?=$_SERVER['PHP_SELF'] . "?action=SW_3_on" ?>"> 
<button><img src="onoff.png" alt="onoff" style="vertical-align: middle"> SW3 ON </button></p></a></p>   
    </td> 
                   <td><p><a href="<?=$_SERVER['PHP_SELF'] . "?action=SW_4_on" ?>"> 
<button><img src="onoff.png" alt="onoff" style="vertical-align: middle"> SW4 ON </button></p></a></p>  
     
</td> 
-->
                 </tr> 
                 <tr> 
                   <td><p><a href="<?=$_SERVER['PHP_SELF'] . "?action=SW_1_off" ?>"> 
<button><img src="onoff.png" alt="onoff" style="vertical-align: middle"> SW1 OFF </button></p></a></p>  
    </td> 
                   <td><p><a href="<?=$_SERVER['PHP_SELF'] . "?action=SW_2_off" ?>"> 
<button><img src="onoff.png" alt="onoff" style="vertical-align: middle"> SW2 OFF </button></p></a></p>  
    </td> 
<!--
                   <td><p><a href="<?=$_SERVER['PHP_SELF'] . "?action=SW_3_off" ?>"> 
<button><img src="onoff.png" alt="onoff" style="vertical-align: middle"> SW3 OFF </button></p></a></p> 
    </td> 
                   <td><p><a href="<?=$_SERVER['PHP_SELF'] . "?action=SW_4_off" ?>"> 
<button><img src="onoff.png" alt="onoff" style="vertical-align: middle"> SW4 OFF </button></p></a></p>  
    </td>
--> 
                 </tr> 
               </table> 
  

     
</body>  
</html>
