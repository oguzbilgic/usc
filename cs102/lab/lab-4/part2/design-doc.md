Overview
--------

This software helps USC basketball team sell discounted tickets. There are five different types of tickets each with a different cost:

+ (A) Students without an activity card	$2.00
+ (B) Faculty and staff	$3.00
+ (C) USC alumni	$5.00
+ (D) UCLA students and alumni	$20.00
+ (E) Everyone else	$10.00

Features
--------

+ User can buy new ticets
+ User can print his order

Classes 
-------

+ TicketOrder Class
  * is single instance of one purchase
  * has these variables
    - char type
    - int quantity  
  * has these methods
    - void TicketOrder(char)
    - int total_cost()
    - char get_type()

Global Data/Functions
---------------------

+ main.cpp
  - int request_quantitiy()
  - char request_command()
  - void print_bill()
  - void print_menu()

High-level Architecture
-----------------------

+ The only class in this software is TicketOrder class.
+ This class all the information about single ticket purchase.
+ This class contains ticket quantity and ticket type.
+ This class has 2 methods.
+ Main function creates this object and store them in single vector.
+ This vector stores all sold tickets
+ In main file there is a print_bill function.
+ This function prints the bill based on the information in this vector.

