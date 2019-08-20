#pragma once
#include "boost/thread/mutex.hpp"
#include "boost/thread/locks.hpp"
#include "boost/thread/thread.hpp"
#include "Queue"

/*
<<���� ���� ����>>
1. Singleton
2. ���������� ��� �����ϵ��� �غ� ��
- ���ٳ� �Լ������͸� �̿��ؼ� POOL ���� �� �ʱ�ȭ ��� ��� ����
- ��ü ���� ��� ���� ��� �� �� �ֵ��� �� ��
3. Queue�� �а� ���� �Ҷ� CS ������ ��
4. �ʱ�ȭ�ÿ��� default ����ŭ�� ����
5. MAX ��ġ�� �Ѿ ��� ��ȯ�̳� MAXġ���� ������ ��� ������Ʈ�� �� ��
*/
template<typename T>
class CustomObjectPool
{
public:
	static CustomObjectPool& instance(int nSize, int maxSize, std::function<T* (T*)> gMethod, std::function<T* (T*)> resetMethod) {
		//C++11������ ���� ���� ���� �ʱ�ȭ �ڵ尡 ��Ƽ������ ȯ�濡���� �� �ѹ� ����
		//thread-safe
		
		static CustomObjectPool* instance = new CustomObjectPool(nSize,maxSize, gMethod, resetMethod);
		
		return *instance;
	}

	T& getObject()
	{
		if (pool.empty())
		{
			if (existObjectCnt < mMaxSize)
			{ 
				{
					boost::lock_guard<boost::mutex> lock(oMutex);
					existObjectCnt++;
				}
				return mGMethod(new T);
			}
			else {
				
				while (true)
				{
					//mutex ����
					{
						boost::lock_guard<boost::mutex> lock(oMutex);
						//pool�� ��ȯ�� ������Ʈ �����
						if (!pool.empty())
						{
							return pool.pop();
						}
						//pool�� ����� �ִ�ġ��ŭ ���� ���� �ʾ��� ��
						else if (existObjectCnt < mMaxSize)
						{
							existObjectCnt++;
							return mGMethod(new T);
						}
						
					}
					boost::this_thread::sleep_for(boost::chrono::milliseconds(30));
					
				}
				
			}
		}
		else{
			boost::lock_guard<boost::mutex> lock(oMutex);
			return pool.pop();
		}
		
	}

	void returnObject(T* object)
	{
		mResetMethod(object);
		{
			boost::lock_guard<boost::mutex> lock(oMutex);
			if (pool.size() < mSize)
				pool.push(mResetMethod(object));
			else {
				delete object;
				existObjectCnt--;
			}
		}
	}

private:
	CustomObjectPool() = default;
	
	//�Ű����� : �⺻ũ��, �ִ�ũ��, ������Ʈ ���� �Լ� ������, ������Ʈ �ʱ�ȭ �Լ� ������
	CustomObjectPool(int nSize,int maxSize,std::function<T* (T*)> gMethod, std::function<T* (T*)> resetMethod)
		:mSize(nSize),mMaxSize(maxSize),mResetMethod(resetMethod),mGMethod(gMethod)
	{
		
		for (int i = 0; i < mSize; i++)
		{
			pool.push(mGMethod(new T));
		}
		
	}

	std::queue<T*> pool;
	int mSize, mMaxSize;
	int existObjectCnt;
	boost::mutex oMutex;
	std::function<T* (T*)> mGMethod;
	std::function<T* (T*)> mResetMethod;
};