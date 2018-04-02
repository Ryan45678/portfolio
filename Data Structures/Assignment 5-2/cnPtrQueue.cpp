#include "cnPtrQueue.h"
#include <cassert>
using namespace std;

namespace CS3358_FA16_A5P2
{
    // constructor
    cnPtrQueue::cnPtrQueue(): numItems(0){}

    // return whether queue is empty or not
    bool cnPtrQueue::empty() const
    {
        // queue is empty when there is no item in the outStack
        // or the inStack
        return (outStack.empty() && inStack.empty());
    }

    // returns # of items in queue
    cnPtrQueue::size_type cnPtrQueue::size() const
    {
        return numItems;
    }

    // returns element at front of queue
    CNode* cnPtrQueue::front()
    {
        // the queue cannot be empty
        assert(!inStack.empty() || !outStack.empty());
        // move inStack to outStack in order to access the front
        // element of the queue
        if (outStack.empty())
        {
            while (!inStack.empty())
            {
                outStack.push(inStack.top());
                inStack.pop();
            }
        }
        // return the front element of the queue
        return outStack.top();
    }

    // pushes item to inStack
    void cnPtrQueue::push(CNode* cnPtr)
    {
        inStack.push(cnPtr);
        numItems++;
    }

    // removes the front element from the queue
    void cnPtrQueue::pop()
    {
        // the queue cannot be empty
        assert(!inStack.empty() || !outStack.empty());
        // move inStack to outStack in order to access the front
        // element of the queue
        if (outStack.empty())
        {
            while (!inStack.empty())
            {
                outStack.push(inStack.top());
                inStack.pop();
            }
        }
        // remove the front item from the queue
        outStack.pop();
        numItems--;
    }
}
