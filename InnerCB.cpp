//Akash Goswami
//CMSC 341 Project 1 Circular Buffer of Circular Buffers                                                                            
//Neary
#include <iostream>
#include "InnerCB.h"
#include <stdexcept>

using namespace std;

InnerCB::InnerCB(int n) {

  m_buffer = new int[n];
  m_capacity = n;
  m_size = 0;
  m_start = 0;
  m_end = 0;
}

InnerCB::InnerCB(const InnerCB& other) {

  //dynamically allocates memory                                                                                                                                                     
  m_buffer = new int[other.m_capacity];

  int index = other.m_start;

  //loops through to copy elements                                                                                                                                                   
  for (int i = 0; i < other.m_size; i++) {

    m_buffer[index] = other.m_buffer[index];

    index++;

    //checks if next index is at front of buffer                                                                                                                                     
    if (index == other.m_capacity)
      index = 0;
  }

  //copies other instance variables                                                                                                                                                  
  m_capacity = other.m_capacity;
  m_size = other.m_size;
  m_start = other.m_start;
  m_end = other.m_end;
}

InnerCB::~InnerCB() {

  //deletes pointer                                                                                                                                                                  
  delete[] m_buffer;
  m_buffer = NULL;

  //sets everything to NULL                                                                                                                                                          
  m_capacity = NULL;
  m_size = NULL;
  m_start = NULL;
  m_end = NULL;
}

void InnerCB::enqueue(int data) {

  //checks if buffer is full                                                                                                                                                         
  if (isFull())
    throw overflow_error("Buffer is full, cannot add");
  else {

    //checks if buffer is empty                                                                                                                                                      
    if (isEmpty())
      m_buffer[m_start] = data;
    else {

      //checks if end position is at end of array                                                                                                                                    
      if (m_end == (m_capacity-1))
        m_end = 0;
      else
        m_end++;

      //adds data to queue                                                                                                                                                           
      m_buffer[m_end] = data;
    }

    //increases # of elements                                                                                                                                                        
    m_size++;
  }
}

int InnerCB::dequeue() {

  //checks if buffer is empty                                                                                                                                                        
  if (isEmpty())
    throw underflow_error("Buffer is empty, cannot remove");
  else {

    //saves data to return and removes data                                                                                                                                          
    int data = m_buffer[m_start];
    m_buffer[m_start] = NULL;

    //checks if start position is at end of array                                                                                                                                    
    if (m_start == (m_capacity-1))
      m_start = 0;
    else
      m_start++;

    //decreases # of elements                                                                                                                                                        
    m_size--;

    return data;
  }
}

bool InnerCB::isFull() {
  return m_capacity == m_size;
}

bool InnerCB::isEmpty() {
  return m_size == 0;
}

int InnerCB::capacity() {
  return m_capacity;
}

int InnerCB::size() {
  return m_size;
}

const InnerCB& InnerCB::operator=(const InnerCB& rhs) {

  //checks for self-assignment                                                                                                                                                       
  if (&rhs == this)
    return *this;

  //deallocates host object                                                                                                                                                          
  delete[] m_buffer;
  m_buffer = NULL;
  m_capacity = NULL;
  m_size = NULL;
  m_start = NULL;
  m_end = NULL;

  m_buffer = new int[rhs.m_capacity];

  //keeps track of index                                                                                                                                                             
  int index = rhs.m_start;

  for (int i = 0; i < rhs.m_size; i++) {

    //sets lhs data equal to rhs data                                                                                                                                                
    m_buffer[index] = rhs.m_buffer[index];

    index++;

    //checks if next index is at front of buffer                                                                                                                                     
    if (index == rhs.m_capacity)
      index = 0;
  }

  //copies instance variables                                                                                                                                                        
  m_capacity = rhs.m_capacity;
  m_size = rhs.m_size;
  m_start = rhs.m_start;
  m_end = rhs.m_end;

  return *this;
}

void InnerCB::dump() {

  cout << "InnerCB dump(): m_size =  " << m_size << endl;

  //keeps track of index                                                                                                                                                             
  int index = m_start;

  //loops same # of times as # of elements                                                                                                                                           
  for (int i = 0; i < m_size; i++) {

    //prints out data with proper format                                                                                                                                             
    cout << "[" << index << "] " << m_buffer[index] << ", ";

    index++;

    //checks if next index is at front of buffer                                                                                                                                     
    if (index == m_capacity)
      index = 0;
  }
  cout << endl;
}
