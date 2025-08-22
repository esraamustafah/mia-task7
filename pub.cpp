#include <ros/ros.h>
#include <geometry_msgs/Twist.h> //for speeds
#include<termios.h> //to read without "enter"
#include <unistd.h> //this reads , read()
#include <iostream> //to use std::cout

  char getch() { 


	 char buf =0; //store each letter entered 
	 struct termios old = {0}; //intailize all to 0
	 
	 if (tcgetattr (0, &old) <0) //tcgetattr:get terminal attributes , <0 means there're errors
	   perror("tcgetattr()"); //prints error

	 old.c_lflag &= ~ICANON; //don't press "enter" , btn by btn
	 old.c_lflag &= ~ECHO; //don't print the button preesed on the screen
	 old.c_cc[VMIN]=1; //wait for a letter
	 old.c_cc[VTIME]=0; // wait to enter a letter
	   
	   if (tcsetattr(0,TCSANOW ,&old ) < 0) //tcsetattr: set terminal attributes, TCSANOW means change now
	     perror("tcsetattr ICANON"); 
	      
	   if (read(0 , &buf , 1) <0 ) //read a letter and store it in buf
	     perror("read()"); //if failed , print error
	     
	   if (tcsetattr(0 ,TCSADRAIN , &old) <0 ) //returns to use "enter"
	     perror ("tcsetattr ~ICANON"); // if failed , print the error
	     
	 return buf;
     
  }
	    
int main(int argc , char **argv ){

	  ros::init(argc , argv , "cmd_vel_pub");
	  ros::NodeHandle n; //act as a gate for connection with the master
	  ros::Publisher cmd_pub = n.advertise<geometry_msgs::Twist>("cmd_vel" , 1000);  //create the publisher as an obj 
	  geometry_msgs::Twist vel_msgs; //the msg with the speed
	  std::cout<<"Use W/A/S/D to move the TurtleBot, Q to quit.\n"; //print this on the terminal
	  
	while (ros::ok()) {
	
	  char c = getch() ; //wait for a letter
	  vel_msgs.linear.x=0; //at beginnig , let th speed =0
	  vel_msgs.angular.z=0; //its rotation = 0
	  
	    switch(c) {
	      case 'w' : vel_msgs.linear.x=0.2; break; //forward
	      case 's' : vel_msgs.linear.x=-0.2;break; //backward
	      case 'a' : vel_msgs.angular.z=0.5;break; //left
	      case 'd' : vel_msgs.angular.z=-0.5;break;   //right
	      case 'q' :return 0;
	      default: break;
	      }
	      
	  cmd_pub.publish(vel_msgs); //send the messages to the topic 
	  ros::spinOnce();
	  }
	  return 0;
	  }    