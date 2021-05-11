// Splitting policy used - split if required required size is less than the size of block allocated using "First-Fit" or "Best-Fit"
// coalescing policy used - coalesce each time free is called

#include<bits/stdc++.h>
using namespace std;

struct block
{
    int size, addr;
    bool allocBit;
    string strData;
    struct block* prev, *next;
}*head,*tail;

void firstFit(string data, int ch);     // Function for First-fit algorithm
void bestFit(string data, int ch);      // Function for Best-fit algorithm
void _free(int addr);                   // Function for freeing a block
void displImplicitList();               // Function for displaying implicit list (allocated + free)
void displAllocatedList();              // Function for displaying allocated list 
void displFreeList();                   // Function for displaying free list
int coalesceFit(int reqBlocks);         // Function for coalescing blocks to allocate space using First-fit or Best-fit
void coalesceFree(struct block* temp);  // Function for coalescing free blocks adjacent to the block being freed

int main()
{
    //creation of 10 blocks(Initially size of each block is 4 Bytes)
    struct block* temp;
    for(int i=0; i<40; i++)
    {
        struct block * temp1 = new block();
        temp1->size = 1;
        temp1->addr = i+1;
        temp1->allocBit = 0;

        if(i==0)
        {
            temp1->prev = NULL;
            temp1->next = NULL;

            head = temp1;
            temp = temp1;
        }
        else
        {
            temp->next = temp1;
            temp1->prev = temp;
            temp1->next = NULL;
            temp = temp->next;
        }
    }
    tail = temp;

    // Creating and Displaying simulation of initial implicit list
    temp = head;
    cout<<"\n---------------Initial heap---------------\n\n";
    while(temp!=NULL)
    {
        temp->size = 4;
        temp->next->next->next->size = 4;
        cout<<"\t"<<temp->addr<<" [4/0][   ][   ][4/0]\n";   
        temp = temp->next->next->next->next;
    }
    cout<<"\n------------------------------------------\n";

    //Menu
    int choice;
    while(1)
    {
        cout<<"\nPress 1 for malloc using First-Fit";
        cout<<"\nPress 2 for malloc using Best-Fit";
        cout<<"\nPress 3 to free";
        cout<<"\nPress 4 to display implicit list(allocated + free)";
        cout<<"\nPress 5 to display allocated list";
        cout<<"\nPress 6 to display free list";
        cout<<"\nPress 0 to exit";
        cout<<"\nPlease enter your choice: ";
        cin>>choice;

        if(choice == 0) // Exiting the menu
            break;

        switch (choice)
        {
            case 1:
            {
                int ch;
                string data;
                cout<<"\nPress 1 to enter a short value";
                cout<<"\nPress 2 to enter an integer value";
                cout<<"\nPress 3 to enter a double value";
                cout<<"\nPress 4 to enter a float value";
                cout<<"\nEnter your choice: ";
                cin>>ch;
                
                if(ch == 1)
                    cout<<"\nEnter a short value : ";
                else if(ch == 2)
                    cout<<"\nEnter an integer value : ";
                else if(ch ==3)
                    cout<<"\nEnter a double value : ";
                else if(ch==4)
                    cout<<"\nEnter a float value : ";
                else
                {
                    cout<<"\n-----------------------------------------------------------\n";
                    cout<<"\nYou have made a wrong choice!!! Please enter a valid choice\n";
                    cout<<"\n-----------------------------------------------------------\n";
                    break;
                }
                
                cin>>data;
                firstFit(data,ch);
                break;
            }
            case 2:
            {
                int ch;
                string data;
                cout<<"\nPress 1 to enter a short value";
                cout<<"\nPress 2 to enter an integer value";
                cout<<"\nPress 3 to enter a double value";
                cout<<"\nPress 4 to enter a float value";
                cout<<"\nEnter your choice: ";
                cin>>ch;
                
                if(ch == 1)
                    cout<<"\nEnter a short value : ";
                else if(ch == 2)
                    cout<<"\nEnter an integer value : ";
                else if(ch == 3)
                    cout<<"\nEnter a double value : ";
                else if(ch == 4)
                    cout<<"\nEnter a float value : ";
                else
                {
                    cout<<"\n-----------------------------------------------------------\n";
                    cout<<"\nYou have made a wrong choice!!! Please enter a valid choice\n";
                    cout<<"\n-----------------------------------------------------------\n";
                    break;
                }
                
                cin>>data;
                bestFit(data, ch);
                break;
            }   
            case 3:
            {
                int addr;
                cout<<"\nEnter the starting address of the data to be freed: ";
                cin>>addr;
                _free(addr);
                break;
            }
            case 4:
            {
                displImplicitList();
                break;
            }
            case 5:
            {
                displAllocatedList();
                break;
            }
            case 6:
            {
                displFreeList();
                break;
            }
            default:
            {
                cout<<"\n-----------------------------------------------------------\n";
                cout<<"\nYou have made a wrong choice!!! Please enter a valid choice\n";
                cout<<"\n-----------------------------------------------------------\n";
                break;
            }
        }
    }
}

void firstFit(string data, int ch)
{
    int reqBlocks;
    if(ch == 1)
        reqBlocks = 2;  // 2 Bytes for short
    else if(ch == 2)
        reqBlocks = 4;  // 4 Bytes for integer
    else if(ch ==3) 
        reqBlocks = 8;  // 8 Bytes for double
    else if(ch == 4)
        reqBlocks = 16; // 16 Bytes for float 

    struct block *temp = head;
    int found = 0;
    while(temp!=NULL)   // Searching first suitable block having size >= required size
    {
        if(temp->allocBit == 0)
        {
            if((temp->size)-2 >= reqBlocks)
            {
                found = 1;
                break;
            }
        } 
        int i = temp->size;
        while(i>=1)
        {
            temp = temp->next;
            i--;
        }
    }

    if(found == 1)  // if suitable block is found
    {
       struct block *temp1 = temp;
       int remaining = temp->size - 2 - reqBlocks;

       int i = temp->size;
       while(i>1)
       {
           temp1 = temp1->next;
           i--;
       }

        temp->allocBit = 1;
        temp->strData = data;
        
        temp->size = reqBlocks + 2;
        temp1->size = temp1->size - temp->size;
        
        i = temp->size;
        while(i>1)
        {
            temp = temp->next;
            i--;
        }

        temp->size = reqBlocks + 2;
        temp->allocBit = 1;
        
        if(temp->next != NULL  && remaining != 0)   // split, if available block size > required size
        {
            temp = temp->next;
            temp->size = temp1->size;
            if(temp1->size==2)
            {
                temp1->allocBit = 1;
                temp->allocBit = 1;
            }
            else
            {
                temp1->allocBit = 0;
                temp->allocBit = 0;
            }
        }
    }
    else    // suitable block is not found
    {
        int availability = coalesceFit(reqBlocks);  // Coalescing the free blocks to achieve suitable block of size >= required size
        if(availability == 1)
        {
            firstFit(data, ch);
        }   
        else    // sufficient space is not available to allocate the memory for required size even after coalescing free blocks
        {
            cout<<"\n-------------------------------------------------------------\n";
            cout<<"\nSorry, the memory for the data "<<data<<" cannot be allocated\n";
            cout<<"\n-------------------------------------------------------------\n";
        }
            
    }
}

void bestFit(string data, int ch)
{
    int reqBlocks;
    if(ch == 1)
        reqBlocks = 2;  // 2 Bytes for short
    else if(ch == 2)
        reqBlocks = 4;  // 4 Bytes for integer
    else if(ch ==3) 
        reqBlocks = 8;  // 8 Bytes for double
    else if(ch == 4)
        reqBlocks = 16; // 16 Bytes for float

    struct block *temp, *temp1;
    temp = head;
    int found = 0;
    int min = 38;
    while(temp != NULL) // Searching the whole list for best-fit
    {
        if(temp->allocBit == 0)
        {
            if(temp->size - 2 >= reqBlocks && temp->size - 2 <min)
            {
                min = temp->size -2;
                temp1 = temp;
                found = 1;
            }
        }
        int i = temp->size;
        while(i>=1)
        {
            temp = temp->next;
            i--;
        }
    }

    if(found == 1)  // if the best-fit is found 
    {
        struct block * temp2 = temp1;
        int remaining = temp1->size - 2 - reqBlocks;
        int i = temp1->size;
        

        while(i>1)
        {
            temp2 = temp2->next;
            i--;
        }

        temp1->allocBit = 1;
        temp1->strData = data;
        
        temp1->size = reqBlocks + 2;
        temp2->size = temp2->size - temp1->size;
        
        i = temp1->size;
        while(i>1)
        {
            temp1 = temp1->next;
            i--;
        }

        temp1->size = reqBlocks + 2;
        temp1->allocBit = 1;

        if(temp1->next != NULL && remaining != 0)   // split, if available block size > required size
        {
            temp1 = temp1->next;
            temp1->size = temp2->size;
            if(temp2->size==2)
            {
                temp2->allocBit = 1;
                temp1->allocBit = 1;
            }
            else
            {
                temp2->allocBit = 0;
                temp1->allocBit = 0;
            }
        }
    }
    else    // suitable block is not found
    {
        int availability = coalesceFit(reqBlocks);  // Coalesce free blocks to achieve suitable block of size >= required size
        if(availability == 1)
            bestFit(data, ch);
        else    // sufficient space is not available to allocate the memory for required size even after coalescing of free blocks
        {
            cout<<"\n-------------------------------------------------------------\n";
            cout<<"\nSorry, the memory for the data "<<data<<" cannot be allocated\n";
            cout<<"\n-------------------------------------------------------------\n";
        }   
    }
}

void _free(int addr)
{
    struct block *temp = head;
    int flag = 0;
    while(temp != NULL) // finding whether "addr" is starting address of the block or not 
    {
        if(temp->addr == addr)
        {
            flag = 1;
            break;
        }
        if(temp->addr > addr)
            break;

        int i = temp->size;
        while(i>=1)
        {
            temp = temp->next;
            i--;
        }
    }

    if(flag == 1)   // if "addr" is the starting address of the block of memory then free that block and coalesce with adjacent free blocks
    {
        if(temp->size == 2)
        {
            if((temp->prev != NULL && temp->prev->allocBit == 0) || (temp->next->next != NULL && temp->next->next->allocBit == 0))
            {
                coalesceFree(temp);
            }
        }
        else if(temp->allocBit != 0)
            coalesceFree(temp);
    }  
    else    
    {
        cout<<"\n--------------------------------------------------------------\n";
        cout<<"\nPlease enter a valid starting address of block to be freed !!!\n";
        cout<<"\n--------------------------------------------------------------\n";
    }      
}

void displImplicitList()
{
    cout<<"\n---------------Displaying implicit list---------------\n\n";
    struct block * temp = head;
    while(temp!=NULL)   // Traversing and printing list with starting address, header, footer and data content
    {
        if(temp->allocBit == 1)
        {
            if(temp->size == 2)
            {
                cout<<"\t"<<temp->addr<<" ["<<temp->size<<"/"<<temp->allocBit<<"]"<<"["<<temp->size<<"/"<<temp->allocBit<<"]\n";
            }
            else
            {        
                cout<<"\t"<<temp->addr<<" ["<<temp->size<<"/"<<temp->allocBit<<"]";
                cout<<"[ "<<temp->strData<<" ]";
                cout<<"["<<temp->size<<"/"<<temp->allocBit<<"]\n";
            }
        }
        else
        {
            cout<<"\t"<<temp->addr<<" ["<<temp->size<<"/"<<temp->allocBit<<"]";
            int i = temp->size;
            for(int j=1; j<=i-2; j++)
                cout<<"[   ]";
            cout<<"["<<temp->size<<"/"<<temp->allocBit<<"]\n";
        }

        int i = temp->size;
        while(i>=1)
        {
            temp = temp->next;
            i--;
        }
    }
    cout<<"\n------------------------------------------------------\n";
}

void displAllocatedList()
{
    cout<<"\n---------------Displaying allocated list---------------\n\n";
    struct block * temp = head;
    while(temp!=NULL)   // Traversing and printing only allocated blocks in the implicit list
    {
        if(temp->allocBit == 1)
        {
            if(temp->size == 2)
            {
                cout<<"\t"<<temp->addr<<" ["<<temp->size<<"/"<<temp->allocBit<<"]"<<"["<<temp->size<<"/"<<temp->allocBit<<"]\n";
            }
            else
            {
                cout<<"\t"<<temp->addr<<" ["<<temp->size<<"/"<<temp->allocBit<<"]";
                cout<<"[ "<<temp->strData<<" ]";
                cout<<"["<<temp->size<<"/"<<temp->allocBit<<"]\n";
            }
        }

        int i = temp->size;
        while(i>=1)
        {
            temp = temp->next;
            i--;
        }
    }
    cout<<"\n-------------------------------------------------------\n";
}

void displFreeList()
{
    cout<<"\n---------------Displaying free list---------------\n\n";
    struct block * temp = head;
    while(temp != NULL) // Traversing and printing the free blocks available in the implicit list
    {
        if(temp->allocBit == 0)
        {
            cout<<"\t"<<temp->addr<<" ["<<temp->size<<"/"<<temp->allocBit<<"]";
            int i = temp->size;
            for(int j=1; j<=i-2; j++)
                cout<<"[   ]";
            cout<<"["<<temp->size<<"/"<<temp->allocBit<<"]\n";
        }

        int i = temp->size;
        while(i>=1)
        {
            temp = temp->next;
            i--;
        }
    }
    cout<<"\n--------------------------------------------------\n";
}

int coalesceFit(int reqBlocks)
{
    struct block * temp, *temp1;
    temp = head;

    int blockNum = 0, flag = 0, i;
    while(temp != NULL) // Traversing through the list to find whether coalescing of free blocks will fulfill the demand of required space or not
    {
        temp1 = temp;
        
       while(temp!=NULL && (temp->allocBit == 0 || temp->size ==2))
       {
           blockNum = blockNum + temp->size;
           if(blockNum-2 >= reqBlocks)
           {
               flag = 1;
               break;
           }
    
            i = temp->size;
            while(i>=1)
            {
                temp = temp->next;
                i--;
            }
       }

        if(flag == 1)
            break;

       blockNum = 0;

        if(temp!=NULL)
        {
            i = temp->size;
            while(i>=1)
            {
                temp = temp->next;
                i--;
            }
        }
    } 

    if(temp!=NULL)  // flag = 1, Coalescing of free blocks fulfill the demand of required space
    {
        i = temp->size;
        while(i>1)
        {
            temp = temp->next;
            i--;
        }

        temp1->size = blockNum;
        temp1->allocBit = 0;

        temp->size = blockNum;
        temp->allocBit = 0;

        i = temp1->size -1;
        while(i>1)
        {
            temp1 = temp1->next;
            temp1->size = 1;
            temp1->allocBit = 0;
            i--;
        }

        return 1;
    }   
    else    // Coalescing of free blocks does not fulfill the demand of required space
    {
        return 0;
    }  
}

void coalesceFree(struct block *temp)
{
    struct block * temp1, *temp2;
    temp1 = temp;
    temp2 = temp1;

    int i = temp1->size;
    while(i>1)  // Moving to the footer of block to be freed
    {
        temp2 = temp2->next;
        i--;
    }

    temp1->allocBit = 0;
    temp2->allocBit = 0;
    
    if(temp1->prev != NULL) // coalesce with next adjacent free block 
    {
        if(temp1->prev->allocBit == 0 || temp1->prev->size == 2)
        {
            int block = temp1->size;
            temp1->prev->allocBit = 0;

            int i = temp1->prev->size;
            while(i>=1)
            {
                temp1 = temp1->prev;
                i--;
            }

            temp1->allocBit = 0;
            temp1->size = temp1->size + block;
            temp2->size = temp1->size;
        }
    }

    if(temp2->next !=NULL)  // Coalesce with previous adjacent free block
    {
        if(temp2->next->allocBit == 0 || temp2->next->size ==2)
        {
            int block = temp2->size;
            temp2->next->allocBit = 0;

            int i = temp2->next->size;
            while(i>=1)
            {
                temp2 = temp2->next;
                i--;
            }

            temp2->allocBit = 0;
            temp2->size = temp2->size + block;
            temp1->size = temp2->size;
        }
    }
}