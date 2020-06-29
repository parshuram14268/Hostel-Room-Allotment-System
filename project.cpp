#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include <bits/stdc++.h>
using namespace std;

struct student
{
    string name; //to store name of student
    char currentBlock; // A or B or C
    string id;//to store registration id
    float cgpa;//to store his cgpa
    int roomNo;//to store his room number
    bool isBooked;// to store details if he has alloted any room or not.
    struct student *right,*left,*nextll,*nextRoommate; //right and left used in the BST, nextll is used in sorted linked list,
                                                        //nextRoommate is to store address of his next Room-mate.
};

struct room
{
    int room_number; //to store the room number
    int status;//to store 1 if room is booked else 0 if room not booked
    struct room *nextRoom; //to store next room address of same having same bed count (4 types of rooms are there)
    struct student *s; // to store the address of group leader student if someone books the room
};

struct block
{
    char block_name; //To store the block name
    int avail_one,avail_two,avail_three,avail_four;//to store the number of rooms available as per the bed count
    struct block *nextBlock; //to store the address of a new block.
    struct room *one,*two,*three,*four; // to store the address of one bedded two beded ... room heads

};

struct hostel
{
    int total_one,total_two,total_three,total_four; //to store the total number of rooms available as per bed count
    struct block *firstBlock; //to store the address of first block
};

struct room* newRoom(struct room* baseRoom,int roomNo);
struct block *newBlock(struct block *fb,char block_name,int num_beds,int roomNo);
struct hostel *createHostel(struct hostel *base,char block_name,int num_beds,int roomNo);
struct hostel *readHostel(struct hostel *base);
void hostelStatus(struct hostel *base);
void trieTraversal(struct hostel *base,int numBeds);
void roomBooking(struct hostel* base,struct student* llroot,struct student* root);
void trieSearch(struct hostel* base);
struct student *newnode(string id,string name,float cgpa);
struct student* inseart(struct student* node,string id,string name,float cgpa);
void inorder (struct student* root,struct student* llroot,struct student* temp);
bool ifPresent(struct student* root,string id);
struct student *readStudent(struct student* root);
struct student* funcIfPresent(struct student* root);
struct student* nextStudent(struct student* llroot);
void studentDetails(struct student* root);





main()
{
    cout<<"***********WELCOME TO HOSTEL ROOM ALLOTMENT SYSTEM By-: ASUTOSH MOHAPATRA & PARSHURAM KUMAR GUPTA**********"<<endl;
    struct student* root = NULL;
    struct student* llroot =new (nothrow) (struct student);
    struct student* temp = new (nothrow) (struct student);
    root = readStudent(root);

    struct hostel* base = NULL;
    base = readHostel(base);

    inorder(root,llroot,temp);
    llroot = llroot->nextll;

    struct student* currentNode =NULL;
    int operation=-1;

    while(operation!=0)
    {
        cout<<"\n"<<"TO BOOK ROOM ENTER 1"<<endl;
        cout<<"TO SEARCH FOR A ROOM ENTER 2"<<endl;
        cout<<"TO CHECK AVAILABLE ROOMS IN TOTAL ENTER 3"<<endl;
        cout<<"TO CHECK IF A STUDENT HAS ALREADY ALLOTED A ROOM OR NOT ENTER 4"<<endl;
        cout<<"TO CHECK WHO IS THE NEXT STUDENT IN QUEUE FOR COUNCELLING ENTER 5"<<endl;
        cout<<"TO CHECK ALL SUDENT DETAILS ENTER 6"<<endl;

        cout<<"TO TERMINATE PROCESS ENTER 0"<<endl<<endl;
        cout<<"ENTER: ";
        cin>>operation;
        if(operation==1) roomBooking(base,llroot,root);
        else if(operation==2) trieSearch(base);
        else if(operation==3)  hostelStatus(base);
        else if(operation==4) funcIfPresent(root);
        else if(operation==5)
        {
            llroot = nextStudent(llroot);
            cout<<"Next Student In Queue is : "<<llroot->name<<" With Registration Id: "<<llroot->id<<endl;
        }
        else if(operation==6) studentDetails(root);
        else if(operation==0) break;
    }
    cout<<"TASK TERMINATED"<<endl;
}

struct room* newRoom(struct room *baseRoom,int roomNo)
{
    struct room* temp=baseRoom;
    if(baseRoom==NULL)
    {
        temp = new (nothrow) (struct room);
        temp->room_number = roomNo;
        temp->status = 0;
        temp->nextRoom = NULL;
        temp->s = NULL;
        return temp;
    }
    while(baseRoom->nextRoom!=NULL)
        baseRoom= baseRoom->nextRoom;
    baseRoom->nextRoom = newRoom(baseRoom->nextRoom,roomNo);
    return temp;

}
struct block *newBlock(struct block *fb,char block_name,int num_beds,int roomNo)
{
    if (fb==NULL)
    {
        struct block *temp =new (nothrow) (struct block);
        temp->block_name = block_name;
        temp->avail_one = 0;
        temp->avail_two = 0;
        temp->avail_three = 0;
        temp->avail_four = 0;
        temp->nextBlock = NULL;
        temp->one=temp->two=temp->three=temp->four=NULL;
        fb = temp;
    }
    if (fb->block_name!=block_name && fb->nextBlock!=NULL)
        newBlock(fb->nextBlock,block_name,num_beds,roomNo);
    else if(fb->block_name!=block_name && fb->nextBlock==NULL)
        fb->nextBlock = newBlock(fb->nextBlock,block_name,num_beds,roomNo);
    else if(fb->block_name==block_name)
    {
        if(num_beds==1)
        {
            fb->avail_one+=1;
            fb->one = newRoom(fb->one,roomNo);
        }
        else if(num_beds==2)
        {
            fb->avail_two+=1;
            fb->two = newRoom(fb->two,roomNo);
        }
        else if(num_beds==3)
        {
            fb->avail_three+=1;
            fb->three = newRoom(fb->three,roomNo);
        }
        else if(num_beds==4)
        {
            fb->avail_four+=1;
            fb->four = newRoom(fb->four,roomNo);
        }
    }
    return fb;
}

struct hostel *createHostel(struct hostel *base,char block_name,int num_beds,int roomNo)
{
    if(base==NULL)
    {
        struct hostel *temp =new (nothrow) (struct hostel);
        temp-> total_one = 0;
        temp-> total_two = 0;
        temp-> total_three = 0;
        temp-> total_four = 0;
        temp->firstBlock = NULL;
        temp->firstBlock = newBlock(temp->firstBlock,block_name,num_beds,roomNo);
        base = temp;
    }
    else
        newBlock(base->firstBlock,block_name,num_beds,roomNo);

    if(num_beds==1)
        base-> total_one += 1;
    else if(num_beds==2)
        base-> total_two += 1;
    else if(num_beds==3)
        base-> total_three += 1;
    else if(num_beds==4)
        base-> total_four += 1;

    return base;

}
struct hostel *readHostel(struct hostel *base)
{
    ifstream infile;
    infile.open("hostel_room.txt");
    int i=0;
    string s;
    while(!infile.eof()) // To get you all the lines.
        {
	        getline(infile,s); // Saves the line in STRING.
	        if(s.empty())
                continue;
	        vector <string> tokens;
	        stringstream check1(s);
            string intermediate;
            while(getline(check1, intermediate, ' '))
                tokens.push_back(intermediate);
            if (i!=0)
            {
                char block_name = tokens[0][0];
                int num_beds = std::stoi(tokens[1]);
                float cgpa = std::stoi(tokens[2]);
                if(base==NULL)
                {
                    base = createHostel(base,block_name,num_beds,cgpa);
                }
                else
                    createHostel(base,block_name,num_beds,cgpa);
            }
            i++;
        }
    infile.close();
    return base;
}

void hostelStatus(struct hostel *base)
{
    cout<<"Total One Beded Rooms Avail: "<<base->total_one<<endl;
    cout<<"Total Two Beded Rooms Avail: "<<base->total_two<<endl;
    cout<<"Total Three Beded Rooms Avail: "<<base->total_three<<endl;
    cout<<"Total Four Beded Rooms Avail: "<<base->total_four<<endl;
}
void trieTraversal(struct hostel *base,int numBeds)
{

    struct block *tempBlock = base->firstBlock;
    while(tempBlock!=NULL)
    {

        struct room *tempRoom=NULL;
        if(numBeds==1 && tempBlock->avail_one>0)
        {
            cout<<"In Block    "<<tempBlock->block_name<<endl;
            cout<<"Available One Beded Rooms: ";
            tempRoom = tempBlock->one;
            while(tempRoom->nextRoom!=NULL)
            {
                if(tempRoom->status==0)
                    cout<<tempRoom->room_number<<"  ";
                tempRoom=tempRoom->nextRoom;
            }
            cout<<endl;
        }

        if(numBeds==2 && tempBlock->avail_two>0)
        {
            cout<<"In Block    "<<tempBlock->block_name<<endl;
            cout<<"Available two Beded Rooms: ";
            tempRoom = tempBlock->two;
            while(tempRoom->nextRoom!=NULL)
            {
                if(tempRoom->status==0)
                    cout<<tempRoom->room_number<<"  ";
                tempRoom=tempRoom->nextRoom;
            }
            cout<<endl;
        }

        if(numBeds==3 && tempBlock->three>0)
        {
            cout<<"In Block    "<<tempBlock->block_name<<endl;
            cout<<"Available Three Beded Rooms: ";
            tempRoom = tempBlock->three;
            while(tempRoom->nextRoom!=NULL)
            {
                if(tempRoom->status==0)
                    cout<<tempRoom->room_number<<"  ";
                tempRoom=tempRoom->nextRoom;
            }
            cout<<endl;
        }

        if(numBeds==4 && tempBlock->four>0)
        {
            cout<<"In Block    "<<tempBlock->block_name<<endl;
            cout<<"Available Four Beded Rooms: ";
            tempRoom = tempBlock->four;
            while(tempRoom->nextRoom!=NULL)
            {
                if(tempRoom->status==0)
                    cout<<tempRoom->room_number<<"  ";
                tempRoom=tempRoom->nextRoom;
            }
            cout<<endl;
        }
        tempBlock=tempBlock->nextBlock;
    }
}
void roomBooking(struct hostel* base,struct student* llroot,struct student* root)
{
    int numBeds,roomNo;
    char blockName;
    llroot = nextStudent(llroot);
    string id = llroot->id;
    llroot->isBooked=true;
    hostelStatus(base);
    cout<<"\n"<< llroot-> name <<"  "<<llroot->id<<"\n"<<"  How many bedded rooms you want?:  ";
    cin>>numBeds;
    trieTraversal(base,numBeds);
    cout<<" Which Block Do You Want? : Enter The Block name: ";
    cin >> blockName;
    cout<<"Which Room Do You Want? Enter The Room Number : ";
    cin>>roomNo;
    struct block *tempBlock = base->firstBlock;
    struct room *tempRoom=NULL;
    while(tempBlock->block_name!=blockName)
        tempBlock=tempBlock->nextBlock;

    if(numBeds==1)
    {
        base->total_one=base->total_one-1;
        tempRoom = tempBlock->one;
        tempBlock->avail_one = tempBlock->avail_one-1;
        while(tempRoom->room_number!=roomNo)
            tempRoom=tempRoom->nextRoom;
    }
    else if(numBeds==2)
    {
        base->total_two=base->total_two-1;
        tempRoom = tempBlock->two;
        tempBlock->avail_two = tempBlock->avail_two-1;
        while(tempRoom->room_number!=roomNo)
            tempRoom=tempRoom->nextRoom;
    }

    else if(numBeds==3)
    {
        base->total_three=base->total_four-1;
        tempRoom = tempBlock->three;
        tempBlock->avail_three = tempBlock->avail_three-1;
        while(tempRoom->room_number!=roomNo)
            tempRoom=tempRoom->nextRoom;
    }

    else if(numBeds==4)
    {
        base->total_four=base->total_four-1;
        tempRoom = tempBlock->four;
        tempBlock->avail_four = tempBlock->avail_four-1;
        while(tempRoom->room_number!=roomNo)
            tempRoom=tempRoom->nextRoom;
    }
    tempRoom->status=1;
    tempRoom->s = llroot;
    llroot->currentBlock = blockName;
    llroot->roomNo = roomNo;
    int i=numBeds-1;
    while(i>0)
    {
        struct student* currentNode =NULL;
        currentNode = funcIfPresent(root);
        if(currentNode!=NULL and currentNode->isBooked==false)
        {
            currentNode->isBooked=true;
            llroot->nextRoommate = currentNode;
            llroot = currentNode;
            llroot->currentBlock = blockName;
            llroot->roomNo = roomNo;
            cout<<"STATUS:   "<<currentNode->name<< "\t"<< currentNode->id <<"\t Has Successfully Booked Room"<< endl;
            i=i-1;
        }
    }
    cout<<"\n"<<"****IN BLOCK >>"<<tempBlock->block_name<<"<< ROOM NUMBER >>"<< tempRoom->room_number<<"<< IS BOOKED SUCCESSFULLY.****"<<endl;
    struct student *studentDetails = tempRoom->s;
    cout<<"Details Of Occupants In "<<blockName<<" in Room Number :"<<roomNo<<endl;
    while(studentDetails->nextRoommate!=NULL)
    {
        cout<<"NAME:"<<"\t"<<studentDetails->name<<endl;
        cout<<"ID NO:"<<"\t"<<studentDetails->id<<endl;
        cout<<"CGPA:"<<"\t"<<studentDetails->cgpa<<endl;
        studentDetails=studentDetails->nextRoommate;
    }
    cout<<"NAME:"<<"\t"<<studentDetails->name<<endl;
    cout<<"ID NO:"<<"\t"<<studentDetails->id<<endl;
    cout<<"CGPA:"<<"\t"<<studentDetails->cgpa<<endl<<endl;
}
void trieSearch(struct hostel* base)
{
    char blockName;
    int roomNo,bedCount;
    struct block *tempBlock = base->firstBlock;
    struct room *tempRoom=NULL;
    while(true)
    {
        tempBlock = base->firstBlock;

        cout<<" ENTER THE BLOCK NAME: ";
        cin>>blockName;
        while( tempBlock!=NULL &&tempBlock->block_name!=blockName )
            tempBlock=tempBlock->nextBlock;
        if(tempBlock==NULL)
            cout<<"Invalid Choice"<<endl;
        else break;
    }
    while(true)
    {
        cout<<" ENTER THE ROOM NUMBER :";
        cin>>roomNo;
        bool loopbreak=false;
        tempRoom = tempBlock->one;
        while(tempRoom->nextRoom!=NULL)
        {
            if(tempRoom->room_number==roomNo) break;
            tempRoom=tempRoom->nextRoom;
        }
        if(tempRoom->room_number==roomNo) break;
        tempRoom = tempBlock->two;
        while(tempRoom->nextRoom!=NULL)
        {
            if(tempRoom->room_number==roomNo) break;
            tempRoom=tempRoom->nextRoom;
        }
        if(tempRoom->room_number==roomNo) break;

        tempRoom = tempBlock->three;
        while(tempRoom->nextRoom!=NULL)
        {
            if(tempRoom->room_number==roomNo) break;
            tempRoom=tempRoom->nextRoom;
        }
        if(tempRoom->room_number==roomNo) break;
        tempRoom = tempBlock->four;
        while(tempRoom->nextRoom!=NULL)
        {
            if(tempRoom->room_number==roomNo) break;
            tempRoom=tempRoom->nextRoom;
        }
        if(tempRoom->room_number==roomNo) break;
        cout<<"Entered Invalid RoomNo "<<endl;
    }
    if(tempRoom->status==0)
        cout<<"ROOM NOT BOOKED."<<endl<<endl;

    else
    {
        struct student *studentDetails = tempRoom->s;
        cout<<"Details Of Occupants In "<<blockName<<" in Room Number :"<<roomNo<<endl;
        while(studentDetails->nextRoommate!=NULL)
        {
            cout<<"NAME:"<<"\t"<<studentDetails->name<<endl;
            cout<<"ID NO:"<<"\t"<<studentDetails->id<<endl;
            cout<<"CGPA:"<<"\t"<<studentDetails->cgpa<<endl;
            studentDetails=studentDetails->nextRoommate;
        }
        cout<<"NAME:"<<"\t"<<studentDetails->name<<endl;
        cout<<"ID NO:"<<"\t"<<studentDetails->id<<endl;
        cout<<"CGPA:"<<"\t"<<studentDetails->cgpa<<endl<<endl;
    }
}
struct student *newnode(string id,string name,float cgpa)
{
    struct student *temp =new (nothrow) (struct student);
    temp->id = id;
    temp->cgpa =cgpa;
    temp->name= name;
    temp->right=temp->left =temp->nextll=NULL;
    temp->isBooked=false;
    temp->roomNo = 0;
    temp->currentBlock = 'n';
    //cout<<temp->id<<"\t"<<temp->name<<"\t"<<temp->cgpa<<"\n";
    return temp;
}
struct student* inseart(struct student* node,string id,string name,float cgpa)
{
    if(node==NULL) return newnode(id,name,cgpa);
    if( cgpa <= node->cgpa)
        node->left = inseart(node->left,id,name,cgpa);
    else if(cgpa>node->cgpa)
        node->right =inseart(node->right,id,name,cgpa);
    return node;
}

void inorder (struct student* root,struct student* llroot,struct student* temp)
{
    if(root!=NULL)
    {
        inorder(root->right,llroot,temp);
        if(temp->nextll!=NULL)
        {
            llroot = temp->nextll;
            temp->nextll = NULL;
        }
        llroot->nextll = root;
        llroot  =  llroot->nextll;
        inorder(root->left,llroot,temp);
        if(temp->nextll==NULL)
            temp->nextll=llroot;
    }
}

bool ifPresent(struct student* node,string id,struct student* currentNode)
{
    if (node == NULL)
        return false;

    if (node->id== id)
    {
        currentNode->nextll = node;
        return true;
    }

    bool res1 = ifPresent(node->left,id,currentNode);

    if(res1) return true;

    bool res2 = ifPresent(node->right,id,currentNode);

    return res2;
}

struct student *readStudent(struct student* root)
{
    ifstream infile;

    infile.open("student_list.txt");

    int i=0;
    string s;
    while(!infile.eof()) // To get you all the lines.
        {
	        getline(infile,s); // Saves the line in STRING.
	        if(s.empty())
                continue;
	        vector <string> tokens;
	        stringstream check1(s);
            string intermediate;
            while(getline(check1, intermediate, ','))
                tokens.push_back(intermediate);


            if (i!=0)
            {
                string id = tokens[0];
                string name = tokens[2];
                float cgpa = std::stof(tokens[1]);
                if(root==NULL)
                    root = inseart(root,id,name,cgpa);
                else
                    inseart(root,id,name,cgpa);
            }
            i++;
        }
    infile.close();
    return root;

}
struct student* funcIfPresent(struct student* root)
{
    string id;
    cout<<"Enter The Id Number Of Student: ";
    cin>>id;
    struct student* currentNode=new (nothrow) (struct student);
    currentNode->nextll = NULL;
    bool status = ifPresent(root,id,currentNode);

    cout<<"Student With Id "<<id ;
    if(status==true)
    {
        if(currentNode->nextll->isBooked==false)
        {
            cout<<" Status: Not Booked"<<endl;
        }
        else
        {
            cout<<" Status: Already Booked"<<endl;
        }
    }
    else
    {
        cout<<" Status: Not there in database"<<endl;
    }
    return currentNode->nextll;
}
struct student* nextStudent(struct student* llroot)
{
    while(llroot->isBooked==true)
        llroot = llroot->nextll;
    return llroot;
}

void studentDetails(struct student* root)
{
    if(root!=NULL)
    {
        studentDetails(root->right);
        cout<< "NAME: \t"<<root->name<<"\t ID: \t"<< root->id<<"\t CGPA: \t"<<root->cgpa;
        if(root->isBooked)
            cout<<"\t BLOCK NAME: \t"<<root->currentBlock <<"\t ROOM NUMBER: \t"<<root->roomNo<< endl;
        else
            cout<<"\t ROOM BOOKING YET TO BE COMPLETED "<<endl;
        studentDetails(root->left);
    }
}
