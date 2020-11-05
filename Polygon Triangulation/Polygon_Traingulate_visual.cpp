/*
Program 1 for Assignment 2 of OOSD Lab and Theory in which to implement Polygon Triangulation in time complexity of O(n*n)
Logic (Program and Time Complexity Explanation )->
The logic used is that to find the polygon triangulation, we need to find all the valid vertices where we could form a valid trianlge with it's adjacent vertices and hence keep on removing one
trianle at a time, therefore first we preprocess the given vertices to classify all the vertices as either reflex(interior angle > 180), concave(interior angle < 180), ear(a concave vertex which
can be removed as a triangle with it's adjacent vertices). Hence This part takes O(n*n) complexity as we traverse through all the vertices which takes O(n) complexity, and when checking if a
vertex is ear or not,O(n) time would be taken to evaluate those vertices, and hence O(n*n) complexity, to classify the vertices respectively into different types.

After preprocessing all the vertices, we would start removing one vertex at a time from the Ear List and hence make triangles with it's adjacent vertices and remove the vertex from the Ear List and
then check if the nature of the adjacent vertices changes(we would check only for the adjacent vertices as those vertices would be the only one to get affected when one ear vertex is removed, and
hence we save time O(n) by not checking for all the vertices as they won't be effected), and hence O(n) complexity would be taken to verify if those two particular vertices have became Ear or not as
we would need to traverse all the other left vertices,and update those respective list. Hence, Total Complexity in this part would also be O(n*n) [O(n) to traverse through the Ear List, and 2*O(n) to
check if the adjacent matrices have become Ear or not, hence total O(n*n)].

Therefore, Total Complexity = O(n*n) + O(n*n) = O(n*n)
*/

#include <iostream>                                                             // including the required libraries
#include <stdlib.h>                                                             // for the exit function
#include <graphics.h>                                                           // for visualization of the polygon

using namespace std;                                                            // using for cout and other functions

struct Coordinates                                                              // defining a struct to store the coordinates of any point
{
    float X;                    // The X - coordinate
    float Y;                    // The Y - coordinate
};

struct CircularNode                                                                     // A structure corresponding to a circular linked list to store the inputed vertices of the polygon
{
    Coordinates vertex;        // stores the coordinate of the particular vertex
    int index;                  // stores the unique index of that vertex
    CircularNode* Next;                 // pointer to next CircularNode
    CircularNode* Prev;                 // pointer to previous CircularNode
};

struct ReflexNode                                                                     // A structure to store the Relfex vertices of the polygon as a Circular Linked List
{
    Coordinates vertex;        // stores the coordinate of the particular vertex
    int index;                  // stores the unique index of that vertex
    ReflexNode* Next;                 // pointer to next ReflexNode
    ReflexNode* Prev;                 // pointer to previous ReflexNode
};

struct ConcaveNode                                                                     // A structure to store the Concave vertices of the polygon as a Circular Linked List
{
    Coordinates vertex;        // stores the coordinate of the particular vertex
    int index;                  // stores the unique index of that vertex
    ConcaveNode* Next;                 // pointer to next ConcaveNode
    ConcaveNode* Prev;                 // pointer to previous ConcaveNode
};

struct EarNode                                                                     // A structure to store the inputed vertices of the polygon as a Circular Linked List
{
    Coordinates vertex;        // stores the coordinate of the particular vertex
    int index;                  // stores the unique index of that vertex
    EarNode* Next;                 // pointer to next EarNode
    EarNode* Prev;                 // pointer to previous EarNode
};

struct Final                                                                       // A structure to store the final output vertex which have diagonal between them for Polygon Triangulation
{
    Coordinates Vertex1;                        // coordinates of first vertex
    Coordinates Vertex2;                        // coordinates of second vertex
    int index1;                                 // unique index of the first vertex
    int index2;                                 // unique index of the second vertex
    Final* Next;                                // pointer to next output vertex
};

class Polygon                                                                   // defining the class for the polygon on which we would perform triangulation
{
private:                                    // defining the private variables which consist of the head and tail pointer of a circular linked list which would contain the vertices of the polygon
    CircularNode* head;                     // Normal head pointer
    int count;                              // keeps the count of the total vertices
    ReflexNode* Reflex;                   // Reflex vertex head pointer
    ConcaveNode* Concave;                  // Concave vertex head pointer
    EarNode* Ear;                      // Ear vertex head pointer
    Final* Output;                      // Output vertex list head pointer

public:                                                                                                     // defining the public variables which consist of the Constructor and all the required member functions
    Polygon(): head(NULL), Reflex(NULL), Concave(NULL), Ear(NULL), count(0), Output(NULL)                  // the Constructor which initializes the different private variables
    {   }

    void GetVertex(float first, float second);                                                            // function which takes in the input vertices
    CircularNode* NewCircularNode(Coordinates data, int index);                                          // the function which makes new CircularNodes on the basis of the data given and index of the vertex
    ReflexNode* NewReflexNode(Coordinates data, int index);                                             // the function which makes new Reflex Nodes on the basis of the data given and index of the vertex
    ConcaveNode* NewConcaveNode(Coordinates data, int index);                                           // the function which makes new Cpncave Nodes on the basis of the data given and index of the vertex
    EarNode* NewEarNode(Coordinates data,int index);                                                 // the function which makes new Ear Nodes on the basis of the data given and index of the vertex
    Final* NewOutputNode(Coordinates data1,Coordinates data2,int index1,int index2);                 // the function which makes new Output Nodes on the basid of the data given and index of the vertex
    void InsertCircularNode(CircularNode* Temp);                                                 // function which inserts the new vertex into the Head List
    void InsertReflexCircularNode(CircularNode* Temp);                                            // function which inserts new Reflex vertex into the Relfex List
    void InsertConcaveCircularNode(CircularNode* Temp);                                             // function which inserts new Concave vertex into the Concave List
    void InsertEarCircularNode(CircularNode* Temp);                                             // function which inserts new Ear vertex into the Ear List
    void InsertOutputNode(CircularNode* Temp1,CircularNode* Temp2);                                   // function which inserts  new Output vertex into the Output List
    void PolygonTriangulation();                                                              // function to triangulate the instance polygon
    int InteriorAngle(CircularNode* Current);                                               // function which evaluates the interior Angle of a particular vertex
    int EarVertex(CircularNode* Current);                                                        // function which checks a vertex would be a ear vertex
    int Barycentre(Coordinates A,Coordinates B,Coordinates C,Coordinates D);                  // function to evaluate that if a point lies inside a trianle or not
    int CheckVertex(CircularNode* Temp);                                                       // a function which finds what that particular vertex is a type of
    void DeleteReflex(CircularNode* Temp);                                                      // function which deletes a reflex vertex from the reflex list
    void DeleteConcave(CircularNode* Temp);                                                 // function which deletes a Concave vertex from the Concave list
    void DeleteEar();                                                                       // function which deletes a Ear vertex from the Ear list
    void PrintInput();                                                                      // function which prints the input in the desired manner
    void PrintOutput();                                                                     // function which prints the ouput in the desired manner
    void Visualize(float A[][2]);                                                           // function which visualizes the polygon using graphics.h
};

CircularNode* Polygon::NewCircularNode(Coordinates data, int index)     // defining the CircularNode function
{
    CircularNode* NewNode = new  CircularNode;                          // assigning all the required values for the newCircularNode
    NewNode->vertex = data,
    NewNode->index = index;
    NewNode->Next = NULL;
    NewNode->Prev = NULL;

    return NewNode;                                     // returning the pointer to the new node
}

ReflexNode* Polygon::NewReflexNode(Coordinates data, int index)     // defining the Refelx Node function
{
    ReflexNode* NewNode = new ReflexNode;                          // assigning all the required values for the ReflexNode
    NewNode->vertex = data,
    NewNode->index = index;
    NewNode->Next = NULL;
    NewNode->Prev = NULL;

    return NewNode;                                     // returning the pointer to the new Reflex node
}

ConcaveNode* Polygon::NewConcaveNode(Coordinates data, int index)     // defining the ConcaveNode function
{
    ConcaveNode* NewNode = new ConcaveNode;                          // assigning all the required values for the ConcaveNode
    NewNode->vertex = data,
    NewNode->index = index;
    NewNode->Next = NULL;
    NewNode->Prev = NULL;

    return NewNode;                                     // returning the pointer to the new ConcaveNode
}

EarNode* Polygon::NewEarNode(Coordinates data,int index)     // defining the EarNode function
{
    EarNode* NewNode = new EarNode;                          // assigning all the required values for the newCircularNode
    NewNode->vertex = data,
    NewNode->index = index;
    NewNode->Next = NULL;
    NewNode->Prev = NULL;

    return NewNode;                                     // returning the pointer to the new EarNode
}

Final* Polygon::NewOutputNode(Coordinates data1,Coordinates data2,int index1,int index2)             // defining the OutputNode function
{
    Final* NewNode = new Final;                          // assigning all the required values for the newCircularNode
    NewNode->Vertex1 = data1,
    NewNode->Vertex2 = data2,
    NewNode->index1 = index1;
    NewNode->index2 = index2;
    NewNode->Next = NULL;
    return NewNode;                                     // returning the pointer to the new node
}

void Polygon::GetVertex(float first, float second)                               // function which gets the input vertex from the user
{
    CircularNode* Temp;                                                                     // initialised a temporary node for storing the data of the input vertex
    Coordinates data;                                                               // storing the value of the coordinates

    data.X = first;                                                                      // X coordinate
    data.Y = second;                                                                      // y coordinate
    Temp = NewCircularNode(data,count);                                                         // storing the value of the vertex
    InsertCircularNode(Temp);                                                                   // inserting the vertex in the circular linked list

    return;                                                                          // exiting the function
}

void Polygon::InsertCircularNode(CircularNode* Temp)                                        // function that inserts new vertex in the circular linked list
{

    if(head==NULL)                              // if no initial vertex is present
    {
        head = Temp;                            // give the required values
        head->Next = head;                      // as it is a circular linked list
        head->Prev = head;
    }
    else                                        // if atleast one vertex is already present
    {
        CircularNode* Current = head;                   // finding the place where to place the next node
        while(Current->Next!=head)
            Current = Current->Next;
        Current->Next = Temp;                   // assigning the required values
        Temp->Prev = Current;
        Temp->Next = head;
        head->Prev = Temp;
    }
    count++;
    return;                                    // returning
}

void Polygon::InsertReflexCircularNode(CircularNode* Temp)                                   // function that inserts new reflex vertex
{
    ReflexNode* Temp2;
    Temp2 = NewReflexNode(Temp->vertex,Temp->index);
    if(Reflex==NULL)                              // if no initial vertex is present
    {
        Reflex = Temp2;                            // give the required values
        Reflex->Next = Reflex;                      // as it is a circular linked list
        Reflex->Prev = Reflex;
    }
    else                                        // if atleast one vertex is already present
    {
        ReflexNode* Current = Reflex;                   // finding the place where to place the next node
        while(Current->Next!=Reflex)
            Current = Current->Next;
        Current->Next = Temp2;                   // assigning the required values
        Temp2->Prev = Current;
        Temp2->Next = Reflex;
        Reflex->Prev = Temp2;
    }
    return;                                    // returning
}

void Polygon::InsertConcaveCircularNode(CircularNode* Temp)                                   // function that inserts new concave vertex
{
    ConcaveNode* Temp2;
    Temp2 = NewConcaveNode(Temp->vertex,Temp->index);
    if(Concave==NULL)                              // if no initial vertex is present
    {
        Concave = Temp2;                            // give the required values
        Concave->Next = Concave;                      // as it is a circular linked list
        Concave->Prev = Concave;
    }
    else                                        // if atleast one vertex is already present
    {
        ConcaveNode* Current = Concave;                   // finding the place where to place the next node
        while(Current->Next!=Concave)
            Current = Current->Next;
        Current->Next = Temp2;                   // assigning the required values
        Temp2->Prev = Current;
        Temp2->Next = Concave;
        Concave->Prev = Temp2;
    }
    return;                                    // returning
}

void Polygon::InsertEarCircularNode(CircularNode* Temp)                                   // function that inserts new Ear vertex
{
    EarNode* Temp2;
    Temp2 = NewEarNode(Temp->vertex,Temp->index);
    if(Ear==NULL)                              // if no initial vertex is present
    {
        Ear = Temp2;                            // give the required values
        Ear->Next = Ear;                      // as it is a circular linked list
        Ear->Prev = Ear;
    }
    else                                        // if atleast one vertex is already present
    {
        EarNode* Current = Ear;                   // finding the place where to place the next node
        while(Current->Next!=Ear)
            Current = Current->Next;
        Current->Next = Temp2;                   // assigning the required values
        Temp2->Prev = Current;
        Temp2->Next = Ear;
        Ear->Prev = Temp2;
    }
    return;                                    // returning
}

void Polygon::InsertOutputNode(CircularNode* Temp1,CircularNode* Temp2)                                   // function that inserts new Output vertex
{
    Final* temp;

    temp = NewOutputNode(Temp1->vertex,Temp2->vertex,Temp1->index,Temp2->index);
    if(Output==NULL)                              // if no initial vertex is present
    {
        Output = temp;                            // give the required values
        Output->Next = NULL;                      // as it is a circular linked list
    }
    else                                        // if atleast one vertex is already present
    {
        Final* Current = Output;                   // finding the place where to place the next node
        while(Current->Next!=NULL)
            Current = Current->Next;
        Current->Next = temp;                   // assigning the required values
        temp->Next = NULL;
    }
    return;                                    // returning
}

void Polygon::PrintInput()                                                                              // function which prints the inputs in the required manner
{
    CircularNode* Temp = head;                                                            // variable to keep track of the vertices
    cout << "Hence, The Input and Output are as follows:\n";
    cout << "Input:{";
    do                                                                              // implementation for outputing the vertices
    {
        cout << "{" <<Temp->vertex.X << "," << Temp->vertex.Y << "}";
        if(Temp->Next!=head)
            cout << ",";
        else
            cout << "}";
        Temp = Temp->Next;
    }while(Temp!=head);                                                             // as circular linked list and hence until we get back to head
    cout << "\n";
    return;
}

void Polygon::PrintOutput()                                                         // function to print the output index of vertices in the required manner
{
    Final* Temp = Output;                                                      // variable to keep track of the vertices
    cout << "Output:{";
    do                                                                      // implementation for outputing the vertices
    {
        cout << "{" <<Temp->index1 << "," << Temp->index2 << "}";
        if(Temp->Next!=NULL)
            cout << ",";
        else
            cout << "}";
        Temp = Temp->Next;
    }while(Temp!=NULL);                                                     // as circular linked list and hence until we get back to head
    cout << "\n";
    return;
}

void Polygon::PolygonTriangulation()                                            // function that would triangulate the instance polygon
{
    PrintInput();
    if(count < 3)                                   // case when number of vertex is less than 3, hence no polygon
    {
        cout << "Please Enter A Valid Polygon.\nYou have inputted only " << count << " Vertices.\nAtleast Three Vertex are needed to make a valid polygon.\n";
        return;
    }
    if(count == 3)                                  // case when number of vertex is 3, hence traingle only and no polygon triangulation needed.
    {
        cout << "You have inputted a Triangle only.\nNo need to triangulate it.Hence, The output would be Empty:\nOutput:{{}}\n";
        return;
    }
    CircularNode* Temp = head;                                               // variable to pass on all vertices
    int Ans, Ans2;
    do
    {
        Ans = InteriorAngle(Temp);                              // checking the intetior angle of that particular vertex
        if(Ans == 0)                                            // hence storing the vertex accordingly in different groups.
            {
                InsertReflexCircularNode(Temp);
            }
        else
        {
            Ans2 = EarVertex(Temp);                                      // now checking the condition for a ear vertex
            if(Ans2 == 0)
                InsertConcaveCircularNode(Temp);
            else
                InsertEarCircularNode(Temp);
        }
        Temp = Temp->Next;
    } while(Temp!=head);                                                // till all the vertices are traversed
    Ear = Ear->Next;                                     // just doing so to take the next ear vertex as the first one to be removed
    for(int i=0;i<count-3;i++)                      // running only till the number of required diagonals are made and they are fixed depending on the number of vertices
    {
        int flag = 0;                           // for any kind of break in assumption
        EarNode* EarTemp = Ear;                 // temporary variables to check the required conditions
        CircularNode* HeadTemp = head;
        CircularNode* HeadTempPrev;
        CircularNode* HeadTempNext;
        do                                                      // finding the Earr node inside the main head list and hence removing that node from head list and storing the index of adjacent vertices
        {
            if(HeadTemp->index == EarTemp->index)               // if vertex is found
            {
                HeadTemp->Prev->Next = HeadTemp->Next;          // storing the required variables
                HeadTemp->Next->Prev = HeadTemp->Prev;
                HeadTempNext = HeadTemp->Next;
                HeadTempPrev = HeadTemp->Prev;
                InsertOutputNode(HeadTempPrev,HeadTempNext);            // storing the diagonal vertices
                delete HeadTemp;                                    // removing that vertex from head list
                flag = 1;
                break;
            }
            else
                HeadTemp = HeadTemp->Next;                          // until that node is not found
        } while(HeadTemp!=head);

        if(flag == 0)                                                   // if some kind of points are given which don't satisfy the assumpotion
        {
            cout << "ERROR.!!!\nPlease Enter valid coordinates.\n";
            exit(0);                                                        // exiting from the program
        }
        DeleteEar();                                                        // also deleting the Ear node
        int flag1,flag2;
        flag1 = CheckVertex(HeadTempPrev);                                  // now finding the nature of the vertex
        flag2 = CheckVertex(HeadTempNext);
        if(flag1==-1)                                                   // if vertex was previously reflexive
        {
            if(InteriorAngle(HeadTempPrev))                                     // checking if the nature changes due to removal of previous ear vertex
            {
                if(EarVertex(HeadTempPrev))
                {
                    InsertEarCircularNode(HeadTempPrev);                        // hence storing in the Ear vertex if vertex becomes ear vertex
                    Ear = Ear->Prev;                                            // just changing the order for the new vertex
                }
                else
                {
                    InsertConcaveCircularNode(HeadTempPrev);                    // hence storing in the Concave vertex if vertex becomes concave vertex
                    Concave = Concave->Prev;
                }
                DeleteReflex(HeadTempPrev);                                     // hence deleting the reflex vertex respectively
            }
        }

        if(flag1==0)                                                   // if vertex was previously concave
        {
            if(EarVertex(HeadTempPrev))                             //checking condition
            {
                InsertEarCircularNode(HeadTempPrev);                        // storing in the Ear List
                Ear = Ear->Prev;
                DeleteConcave(HeadTempPrev);                                // Deleting from the concave list
            }
        }


        if(flag2==-1)                                                   // if vertex was previously reflexive
        {
            if(InteriorAngle(HeadTempNext))                                     // checking if the nature changes due to removal of previous ear vertex
            {
                if(EarVertex(HeadTempNext))
                {
                    InsertEarCircularNode(HeadTempNext);                        // hence storing in the Ear vertex if vertex becomes ear vertex
                    Ear = Ear->Prev;                                            // just changing the order for the new vertex                        // enter the delete node also
                }
                else
                {
                    InsertConcaveCircularNode(HeadTempNext);                    // hence storing in the Concave vertex if vertex becomes concave vertex
                    Concave = Concave->Prev;
                }
                DeleteReflex(HeadTempNext);
            }
        }

        if(flag2==0)                                                   // if vertex was previously concave
        {
            if(EarVertex(HeadTempNext))                             //checking condition
            {
                InsertEarCircularNode(HeadTempNext);                        // storing in the Ear List
                Ear = Ear->Prev;
                DeleteConcave(HeadTempNext);                                // Deleting from the concave list
            }
        }
    }

    PrintOutput();                                                      // Hence printing the Answer

    return;                                                         // returning
}

void Polygon::Visualize(float A[][2])                                               // function to visualize the polygon using the grahpics.h library
{
    Final* Temp = Output;                                                           // keeping track from the output vertices
    int gd = DETECT, gm;                                                            // completing the required things plotting the ploygon
    initgraph(&gd, &gm, "");
    cleardevice();                                                              // clearing the interface
    setcolor(15);                                                               // setting a colout for the polygon
    for(int i=0;i<count-1;i++)                                                  // plotting first the polygon
        line(A[i][0],A[i][1],A[i+1][0],A[i+1][1]);

    line(A[0][0],A[0][1],A[count-1][0],A[count-1][1]);                          // joining the last vertex to the first one
    setcolor(1);                                                            // setting color for the diagonals
    do
    {
        if(Output == NULL)
         break;
        line(Temp->Vertex1.X,Temp->Vertex1.Y,Temp->Vertex2.X,Temp->Vertex2.Y);
        Temp = Temp->Next;
    } while(Temp!=NULL);                                                        // plotting the diagonals respectively
    getch();
    closegraph();                                                               // closing the graph
    return;                                                         // returning
}

void Polygon::DeleteReflex(CircularNode* Temp)                                                          // function which deletes a relfexive vertex from its list
{
    ReflexNode* Current = Reflex;                                               // keeping track of the node
    if(Reflex->index == Temp->index)                                        // if head node is the node to removed
    {
        if(Reflex->Next == Reflex)                                          // if only one reflex node is left
        {
            delete Reflex;
            Reflex = NULL;
        }
        else                                                            // and if other nodes are also present
        {
            Reflex->Prev->Next = Reflex->Next;
            Reflex = Reflex->Prev;
            delete Reflex->Next->Prev;
            Reflex->Next->Prev = Reflex;
            Reflex = Reflex->Next;
        }
    }
    else                                                                //   finding the required node
    {
        do
        {
            if(Current->index == Temp->index)                                               // when node is found
            {
                Current->Prev->Next = Current->Next;                                // doing the required things
                Current = Current->Prev;
                delete Current->Next->Prev;
                Current->Next->Prev = Current;
                Current = Current->Next;
                break;
            }
            else
                Current = Current->Next;
        } while(Current!=Reflex);                                                       // until that node is found
    }
    return;
}

void Polygon::DeleteConcave(CircularNode* Temp)                                                          // function which deletes a concave vertex from its list
{
    ConcaveNode* Current = Concave;                                              // keeping track of the node
    if(Concave->index == Temp->index)                                        // if head node is the node to removed
    {
        if(Concave->Next == Concave)                                        // if only one reflex node is left
        {
            delete Concave;
            Concave = NULL;
        }
        else                                                           // and if other nodes are also present
        {
            Concave->Prev->Next = Concave->Next;
            Concave = Concave->Prev;
            delete Concave->Next->Prev;
            Concave->Next->Prev = Concave;
            Concave = Concave->Next;
        }
    }
    else                                                               //   finding the required node
    {
        do
        {
            if(Current->index == Temp->index)                                               // when node is found
            {
                Current->Prev->Next = Current->Next;                               // doing the required things
                Current = Current->Prev;
                delete Current->Next->Prev;
                Current->Next->Prev = Current;
                Current = Current->Next;
                break;
            }
            else
                Current = Current->Next;
        } while(Current!=Concave);                                                       // until that node is found
    }
    return;
}

void Polygon::DeleteEar()                                                          // function which deletes a Ear vertex from its list
{                                                                                   // here only this is required as we always remove the firsr ear node
    if(Ear->Next == Ear)                                                    // if only one node is present
    {
        delete Ear;
        Ear = NULL;
    }
    else                                                                    // if others nodes also present
    {
        Ear->Prev->Next = Ear->Next;
        Ear = Ear->Prev;
        delete Ear->Next->Prev;
        Ear->Next->Prev = Ear;
        Ear = Ear->Next;
    }
    return;
}

int Polygon::CheckVertex(CircularNode* Temp)                                            // funuction to find the what nature is a vertex a type of
{
    EarNode* Earcheck = Ear;                                                            // variables for Ear, Concave Node, not required for reflex as if they both wouldn't be then vertex would be reflex
    ConcaveNode* Concavecheck = Concave;
    do
    {
        if(Earcheck == NULL)                                                            // if no ear nodes are present
            break;
        if(Earcheck->index==Temp->index)                                            // if the vertex is ear node
            return 1;
        Earcheck = Earcheck->Next;
    } while(Earcheck!=Ear);                                                             // until all are checked
    do
    {
        if(Concavecheck == NULL)                                                        // if no concave node is present
            break;
        if(Concavecheck->index == Temp->index)                                          // if the vertex is concave
            return 0;
        Concavecheck = Concavecheck->Next;
    }while (Concavecheck!=Concave);                                                 // until all the nodes are checked

    return -1;                                                      // if not ear and concave, then refelx only
}

int Polygon::InteriorAngle(CircularNode* Current)                               // function to evaluate the interior angle of any Vertex
{
    Coordinates ref1, ref2;                                                     // variable to store the temporary coordinates
    ref1.X = Current->Prev->vertex.X - Current->vertex.X;                       // evaluating the vectors for cross product
    ref1.Y = Current->Prev->vertex.Y - Current->vertex.Y;
    ref2.X = Current->Next->vertex.X - Current->vertex.X;
    ref2.Y = Current->Next->vertex.Y - Current->vertex.Y;

    float cross = ref1.X*ref2.Y - ref1.Y*ref2.X;                                // evaluating the cross product
    if(cross > 0)                                                               // returning answer accordingly
        return 1;
    else if(cross < 0)
        return 0;
    else
        {
            cout << "Error!!!.\nYou have given three points as collinear.Please Enter valid Coordinates.\n";
            exit(0);
        }
    return 0;
}

int Polygon::EarVertex(CircularNode* Current)                                                       // function to evaluate of a point is valid to be removed as a triangle
{
    CircularNode* Temp = head;                                                                      // variable to keep track of vertices
    int Ans;                                                                                            // for checking the answer
    do
    {
        if(Temp->index == Current->index || Temp->index == Current->Prev->index || Temp->index == Current->Next->index)     // if the vertex is anyone from the triangle being formed then we skip as that already lies in the triangle
            Temp = Temp->Next;
        else
        {
            Ans = Barycentre(Current->Prev->vertex,Current->vertex,Current->Next->vertex,Temp->vertex);                                                 // algorithm to check the condition
            if(Ans == -1)                                                                                                           // returning the accordingly
            {
                cout << "ERROR.!!!\nYou have given Three Collinear Points as Vertices.\nPlease give valid Coordinates.\n";
                exit(0);
            }
            else if(Ans == 0)                                                   // if any other vertex other than the vertex of the triangle lie inside the triangle
                return 0;
            else
                Temp = Temp->Next;                                              // checking all the vertices
        }
    } while(Temp!=head);                                                        // till all the vertices are verified
    return 1;
}

int Polygon::Barycentre(Coordinates A,Coordinates B,Coordinates C,Coordinates D)         // function to evaluate the barycentre coordinates of the point given wrt traingle
{
    float alpha,beta,gamma;

    float detTri = (B.Y - C.Y)*(A.X - C.X) + (C.X - B.X)*(A.Y - C.Y);           // Evaluating the determinant from the vetices of the triangle

    if(detTri==0)                                                                  // if determinant if zero than points are collinear
        return -1;
    else                                                                        // checking the values of the barycentric coordinates of the point D wrt the triangle
    {
        alpha = ((B.Y - C.Y)*(D.X - C.X) + (C.X - B.X)*(D.Y - C.Y))/detTri;
        beta = ((C.Y - A.Y)*(D.X - C.X) + (A.X - C.X)*(D.Y - C.Y))/detTri;
        gamma = 1 - alpha - beta;
        if(alpha < 0 || beta < 0 || gamma < 0)                                  // if the point lies outside the trianle
            return 1;
        else
            return 0;                                                           // if the point lies inside or on the triangle
    }
}


int main()                                                                      // The main function where the program starts
{
    int n = 23;                                                                 // respectively write here the number of vertices

   float Input[n][2] = {{151,21},                                               // write here the input vertices of the polygon
                        {151,50},                                               // in clockwise manner only
                        {142,78},
                        {118,25},
                        {57,49},
                        {38,39},
                        {27,64},
                        {59,73},
                        {68,129},
                        {109,104},
                        {72,155},
                        {63,223},
                        {93,222},
                        {154,170},
                        {205,220},
                        {221,196},
                        {209,192},
                        {241,173},
                        {240,147},
                        {210,148},
                        {246,133},
                        {181,88},
                        {173,29}};

   Polygon NewPolygon;                                                         // declaring a newplogon instance of the polyong class
   for(int i=0;i<n;i++)
       NewPolygon.GetVertex(Input[i][0],Input[i][1]);                                                     // calling the Getvertex function

    NewPolygon.PolygonTriangulation();                                          // function which would triangulate the given polygon

    NewPolygon.Visualize(Input);

    return 0;                                                                   // returning
}
