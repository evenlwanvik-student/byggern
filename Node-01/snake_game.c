#ifndef F_CPU
#define F_CPU 4915200UL	//This is just a macro, it has no data type.
#endif

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "OLED_driver.h"
#include "joystick_driver.h"

#include "snake_OLED.h"

typedef unsigned char uchar_t;

#define SPEED 5000

// list to record the every turns
struct node
{
		joystick_direction_t dir;	  // direction
		unsigned char body_length;	// the count of the dot in this section
		struct node *next;			    // next node
};

struct snake
{
  	uchar_t head_x;		    // coordinate of the head
  	uchar_t head_y;
  	uchar_t tail_x;		    // coordinate of the tail
  	uchar_t tail_y;
  	joystick_direction_t dir;  // current dirction of the head
    uchar_t level;		   // game level
  #define L0 0x00
  #define L1 0x01
  #define L2 0x02
  #define L3 0x03
  #define L4 0x04
  #define L5 0x05
  #define L6 0x06
  #define L7 0x07
  	unsigned int  body_length;	// number of dots for body
  	uchar_t next_x;	      // coordinate of the destination
  	uchar_t next_y;
  	struct node * p;		        // the head of the list, the head is the earlist turn
} head_data;

uchar_t hit_self(uchar_t x,uchar_t y); //prototype

/* use four pixel as a dot,that means 128*64pixels have 64*32 dots
fill the four pixels with 1 or 0 */
void encode_dot(uchar_t x,uchar_t y,uchar_t val)
{
  	one_pixel(x<<1,y<<1,val);
  	one_pixel(x<<1,(y<<1)+1,val);
  	one_pixel((x<<1)+1,y<<1,val);
  	one_pixel((x<<1)+1,(y<<1)+1,val);
}

// Create a random dot
void create_dot() {
    unsigned int position;
    while (1) {
        srand(TCNT1);
        position = rand()>>4;			              //creat dot
        if (hit_self(position%64,position/64))
            continue;                           //if the dot is not locate at blank spce
        encode_dot(position%64,position/64,1);	    //fill the new dot
        head_data.next_x = position%64;        //set the coordinate of the new dot
        head_data.next_y = position/64;
        break;
    }
}

/* Initialization of the snake,
@(x,y) means the start coordinate
@length is the default length of the snake
@level is the speed of the game */
void snake_init(uchar_t x,uchar_t y,uchar_t length,uchar_t level)
{
		snake_OLED_init();
  	uchar_t i;
  	head_data.head_x = x + length - 1;	   // head of the snake
  	head_data.head_y = y;
  	head_data.tail_x = x;				           // tail of the snake
  	head_data.tail_y = y;
    head_data.dir = RIGHT;	               // directon
  	head_data.level = level;			         // level
  	head_data.body_length = length;		     // length
  	for(i = 0;i<length;i++)
        encode_dot(x+i,y,1);               // fill the snake up til start length
    create_dot();
  	head_data.p = malloc(4); 			 	       // malloc data for head
  	head_data.p->dir = RIGHT;		           // direction
  	head_data.p->body_length = length;		// start length
  	head_data.p->next = 0;					       // no next node
  	update_OLED();							           //refresh the screen
		printf("Snake init ...\n\r");
		_delay_ms(5000);
}

/* if the snake hit itself  1 hit 0 not hit
@x: x coordinate in matrix
@y: y coordinate in matrix
*/
uchar_t hit_self(uchar_t x,uchar_t y)
{
	 return read_pixel(x<<1,y<<1);
}

uint8_t move()
{
    uint8_t pos;
    uchar_t x,y;
    struct node * tmp; // temporary node

    // check if player hits wall
    if      (head_data.dir == RIGHT && head_data.head_x == 63) return 1;
    else if (head_data.dir == LEFT  && head_data.head_x == 0)  return 1;
    else if (head_data.dir == UP    && head_data.head_y == 0)  return 1;
    else if (head_data.dir == DOWN  && head_data.head_y == 31) return 1;

    //set (x,y) according the direction
  	x = head_data.head_x;
  	y = head_data.head_y;
  	if      (head_data.dir == RIGHT) x++;
  	else if (head_data.dir == LEFT)  x--;
  	else if (head_data.dir == UP)    y--;
  	else if (head_data.dir == DOWN)  y++;

    tmp = head_data.head_y;
    while (tmp->next != 0) tmp = tmp->next; // find the last node (the last turn of the snake)
    tmp->body_length++;                     // add the count
		printf("\nnect location: x = %d, y = %d\n\r", x,y);
		printf("current head: x = %d, y = %d\n\n\r", head_data.head_x,head_data.head_y);

    //if the snake hit the destination
  	if(x == head_data.next_x && y == head_data.next_y)
  	{
    		//encode_dot(x,y,1);
    		head_data.body_length += 1;	  //length += 1
    		head_data.head_x = x;	        //set the new coordinate of the head
    		head_data.head_y = y;
        create_dot();
    		update_OLED();			          // refresh
    		return 0;				              // return to new iteration
				printf("\nsup\n\r");
  	}
    // if it hits itself
    else if (hit_self(x,y) &&
             x != head_data.tail_x &&
             y != head_data.tail_y)
		{
						 printf("\ngame over\n\r");
             return 1;                //gameover
		}
    // normal movement
    else
    {
				printf("\nsup\n\r");
        encode_dot(x,y,1);			        // move ahead
        head_data.head_x = x;	          // set the new head coordinate
        head_data.head_y = y;
        encode_dot(head_data.head_x,head_data.head_y,0); // clear the tail
        update_OLED();									  //refresh

        if(head_data.p->body_length == 1)		 	  //if the dot is the last of this turn
        {
            tmp = head_data.p->next;	  //save node
            free(head_data.p);					//delete the node
            head_data.p = tmp;				  //reset
        }
        else                            //otherwise decrement the count of this turn
        {
            head_data.p->body_length -= 1;
        }
        //set the new tail according the direction
        switch(head_data.p->dir)
        {
            case RIGHT:head_data.tail_x++; break;
            case LEFT :head_data.tail_x--; break;
            case UP   :head_data.tail_y--; break;
            case DOWN :head_data.tail_y++; break;
            default:break;
        }
    }
		printf("\nnot game over 2\n\r");
    return 0; // return to new iteration

}

void play_snake()
{
    unsigned int count;           // speed
    joystick_direction_t dir;     // direction
    joystick_direction_t current_dir = head_data.dir;
    struct node * tmp;            // temporary node
    struct node * tmp1;
    switch(head_data.level)      // //set the speed according the level
    {
        case L0: count = SPEED/(L0+1);break;
    		case L1: count = SPEED/(L1+1);break;
    		case L2: count = SPEED/(L2+1);break;
    		case L3: count = SPEED/(L3+1);break;
    		case L4: count = SPEED/(L4+1);break;
    		case L5: count = SPEED/(L5+1);break;
    		case L6: count = SPEED/(L6+1);break;
    		case L7: count = SPEED/(L7+1);break;
    		default: count = SPEED/(L0+1);break;
    }
    while(1)
    {
        dir = joystick_direction_get();         // get current direction from joystick
				if ((dir != 0) && (dir != current_dir)) // if we have a new direction (not neutral)
        {
            // Snake can't turn 180*
            if ((dir == RIGHT && current_dir == LEFT)  ||
                (dir == LEFT  && current_dir == RIGHT) ||
                (dir == UP    && current_dir == DOWN)  ||
                (dir == DOWN  && current_dir == UP));
            else
      			{
        				current_dir = dir;		             // change current direction
        				struct node *p = malloc(4);			   // create new child node
        				p->dir = current_dir;	             // set direction
          			p->body_length = 0;							   // body_length=1 it will be set 1 in function Move()
        				p->next = NULL;						           // no childnode
        				tmp = head_data.p;				         // set the new list head
        				while(tmp->next != NULL) tmp = tmp->next; // find the last node
        				tmp->next = p;						         // add node as children of second last node
      			}
        }
        switch(dir) // set the direction of the head
        {
            case RIGHT : if(head_data.dir != LEFT)  head_data.dir = RIGHT; break;
            case LEFT  : if(head_data.dir != RIGHT) head_data.dir = LEFT;  break;
            case UP    : if(head_data.dir != DOWN)  head_data.dir = RIGHT; break;
            case DOWN  : if(head_data.dir != UP)    head_data.dir = RIGHT; break;
            default: break;
        }
        if (move())
        {
            // game over, release the memory of every node
            tmp = head_data.p;
            while (tmp->next != 0)
            {
                tmp1 = tmp->next;
                free(tmp);
                tmp = tmp1;
            }
            free(tmp);
            return;
        }
    }
}

void main_snake(){
		TCNT1H=0x00; // use the TCNT1 timer as seed
	  TCNT1L=0x00;
	  TCCR1A=0x03; //ctc mode  use icr1
	  TCCR1B=0x01; //system clock;
	  ICR1H=0xff;
	  ICR1L=0xff;
		while(1)
		{
				printf("New game\n\n\r");
				snake_init(10,10,8,0);
				play_snake();
		}
}
