#include "nodes_LLoLL.h"
#include "cnPtrQueue.h"
#include <iostream>
using namespace std;

namespace CS3358_FA16_A5P2
{
   void Destroy_cList(CNode*& cListHead)
   {
      int count = 0;
      CNode* cNodePtr = cListHead;
      while (cListHead != 0)
      {
         cListHead = cListHead->link;
         delete cNodePtr;
         cNodePtr = cListHead;
         ++count;
      }
      cout << "Dynamic memory for " << count << " CNodes freed"
           << endl;
   }

   void Destroy_pList(PNode*& pListHead)
   {
      int count = 0;
      PNode* pNodePtr = pListHead;
      while (pListHead != 0)
      {
         pListHead = pListHead->link;
         Destroy_cList(pNodePtr->data);
         delete pNodePtr;
         pNodePtr = pListHead;
         ++count;
      }
      cout << "Dynamic memory for " << count << " PNodes freed"
           << endl;
   }

   // do depth-first traversal and print data
   void ShowAll_DF(PNode* pListHead, ostream& outs)
   {
      while (pListHead != 0)
      {
         CNode* cListHead = pListHead->data;
         while (cListHead != 0)
         {
            outs << cListHead->data << "  ";
            cListHead = cListHead->link;
         }
         pListHead = pListHead->link;
      }
   }

   // do breadth-first (level) traversal and print data
   void ShowAll_BF(PNode* pListHead, ostream& outs)
   {
       // create a queue
       cnPtrQueue q;
       // start the cursor at the head of the main list
       PNode * cursor = pListHead;
       // add each sublist to the queue
       while (cursor != 0)
        {
           if (cursor->data != 0)
               q.push(cursor->data);

           cursor = cursor->link;
        }

        // traverse the queue
        while (!q.empty())
        {
            // print the front item's value
            outs << q.front()->data << "  ";

            // if there is another item in the front item's list,
            // add it to the back of the queue. Once each sublist
            // is empty, no more items will be added to the queue
            // from that particular list.
            if (q.front()->link != 0)
               q.push(q.front()->link);
            // remove the printed item from the queue
            q.pop();
        }

   }
}
