//Akash Goswami                                                                                                                                                                      
//CMSC 341 Project 1 Circular Buffer of Circular Buffers                                                                                                                             
//Neary                                                                                                                                                                              
#include <iostream>
#include "CBofCB.h"
#include <stdexcept>

using namespace std;

CBofCB::CBofCB() {

  m_buffers[0] = new InnerCB();
  m_obSize = 1;
  m_oldest = 0;
  m_newest = 0;
}

CBofCB::CBofCB(const CBofCB& other) {

  //keeps track of index                                                                                                                                                             
  int index = other.m_oldest;

  //loops through other buffer and copies data                                                                                                                                       
  for (int i = 0; i < other.m_obSize; i++) {

    //copies inner buffer                                                                                                                                                            
    m_buffers[index] = new InnerCB(*other.m_buffers[index]);

    index++;

    //checks if at the end of outer buffer                                                                                                                                           
    if (index == m_obCapacity)
      index = 0;
  }

  //copies instance variables                                                                                                                                                        
  m_obSize = other.m_obSize;
  m_oldest = other.m_oldest;
  m_newest = other.m_newest;
}

CBofCB::~CBofCB() {

  //keeps track of index                                                                                                                                                             
  int index = m_oldest;

  //loops through outer buffer                                                                                                                                                       
  for (int i = 0; i < m_obSize; i++) {

    //deletes pointers                                                                                                                                                               
    delete m_buffers[index];
    m_buffers[index] = NULL;

    index++;

    //checks if index is at end of outer buffer                                                                                                                                      
    if (index == m_obCapacity)
      index = 0;
  }

  //deletes instance variables                                                                                                                                                       
  m_obSize = NULL;
  m_oldest = NULL;
  m_newest = NULL;
}

void CBofCB::enqueue(int data) {

  //checks if outer buffer is full                                                                                                                                                   
  if (isFull())
    throw overflow_error("Cannot add, buffer full");
  else {

    //checks if inner buffer is full                                                                                                                                                 
    if (m_buffers[m_newest]->isFull()) {

      int oldNewest = m_newest;

      //checks if outer buffer is at the end                                                                                                                                         
      if (m_newest == (m_obCapacity-1))
        m_newest = 0;
      else
        m_newest++;

      //creates new inner buffer with twice the length                                                                                                                               
      //of previous inner buffer and adds data                                                                                                                                       
      m_buffers[m_newest] = new InnerCB(m_buffers[oldNewest]->capacity()*2);

      //increases size of outer buffer                                                                                                                                               
      m_obSize++;
    }

    //adds data to outer buffer                                                                                                                                                      
    m_buffers[m_newest]->enqueue(data);
  }
}

int CBofCB::dequeue() {

  //checks if outer buffer is empty                                                                                                                                                  
  if (isEmpty())
    throw underflow_error("Cannot remove, buffer empty");
  else {

    //checks if there is only 1 outer buffer                                                                                                                                         
    if (m_obSize == 1)
      return m_buffers[m_oldest]->dequeue();

    //checks if this buffer only has 1 item                                                                                                                                          
    if (m_buffers[m_oldest]->size() == 1) {

      int data = m_buffers[m_oldest]->dequeue();

      //deallocates and removes this InnerCB                                                                                                                                         
      delete m_buffers[m_oldest];
      m_buffers[m_oldest] = NULL;

      //reduces size of outer buffer                                                                                                                                                 
      m_obSize--;

      //checks if oldest is at end of outer buffer                                                                                                                                   
      if (m_oldest == m_obCapacity-1)
        m_oldest = 0;
      else
        m_oldest++;

      return data;
    }

    //returns removed data                                                                                                                                                           
    return  m_buffers[m_oldest]->dequeue();
  }
}

bool CBofCB::isFull() {

  //checks if max circular buffers made                                                                                                                                              
  if (m_obSize == m_obCapacity) {

    //checks if last circular buffer is full                                                                                                                                         
    return m_buffers[m_newest]->isFull();
  }
  return false;
}

bool CBofCB::isEmpty() {

  //checks if there is 1 inner buffer                                                                                                                                                
  if (m_obSize == 1) {

    //checks if inner buffer is empty                                                                                                                                                
    return m_buffers[m_oldest]->isEmpty();
  }

  return false;
}

int CBofCB::size() {

  //keeps track of size                                                                                                                                                              
  int size = 0;

  //keeps track of index                                                                                                                                                             
  int index = m_oldest;

  //loops through buffer                                                                                                                                                             
  for (int i = 0; i < m_obSize; i++) {

    //adds # of elements                                                                                                                                                             
    size += m_buffers[index]->size();
    index++;

    //checks if index is at end of buffer                                                                                                                                            
    if (index == m_obCapacity)
      index = 0;
  }

  return size;
}

const CBofCB& CBofCB::operator=(const CBofCB& rhs) {

  //checks for self-assignment                                                                                                                                                       
  if (&rhs == this)
    return *this;

  //keeps track of index in lhs                                                                                                                                                      
  int index = m_oldest;

  //deallocates host object                                                                                                                                                          
  for (int j = 0; j < m_obSize; j++) {

    //deletes pointers                                                                                                                                                               
    delete m_buffers[index];
    m_buffers[index] = NULL;

    index++;

    //checks if next index is 0                                                                                                                                                      
    if (index == m_obCapacity)
      index = 0;
  }

  //deletes instance variables                                                                                                                                                       
  m_obSize = NULL;
  m_oldest = NULL;
  m_newest = NULL;

  //keeps track of index in rhs                                                                                                                                                      
  index = rhs.m_oldest;

  //loops through rhs outer buffer                                                                                                                                                   
  for (int i = 0; i < rhs.m_obSize; i++) {

    //copies rhs data                                                                                                                                                                
    m_buffers[index] = new InnerCB(*rhs.m_buffers[index]);

    index++;

    //checks if next rhs index is 0                                                                                                                                                  
    if (index == m_obCapacity)
      index = 0;
  }

  //copies rhs instance variables                                                                                                                                                    
  m_obSize = rhs.m_obSize;
  m_oldest = rhs.m_oldest;
  m_newest = rhs.m_newest;

  return *this;
}

void CBofCB::dump() {

  cout << "Outer Circular buffer dump(), m_obSize = " << m_obSize << ":"  << endl;

  //keeps track of index                                                                                                                                                             
  int index = m_oldest;

  //loops through buffer                                                                                                                                                             
  for (int i = 0; i < m_obSize; i++) {

    //prints out contents                                                                                                                                                            
    m_buffers[index]->dump();

    index++;

    //checks if index is at end of buffer                                                                                                                                            
    if (index == m_obCapacity)
      index = 0;
  }
}