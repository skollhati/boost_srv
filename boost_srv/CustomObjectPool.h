#pragma once
#include "boost/thread/mutex.hpp"
#include "boost/thread/locks.hpp"
#include "boost/thread/thread.hpp"
#include "Queue"

/*
<<개발 주의 사항>>
1. Singleton
2. 범용적으로 사용 가능하도록 해볼 것
- 람다나 함수포인터를 이용해서 POOL 생성 시 초기화 대신 사용 가능
- 객체 종류 상관 없이 사용 할 수 있도록 할 것
3. Queue에 읽고 쓰기 할때 CS 주의할 것
4. 초기화시에는 default 수만큼만 생성
5. MAX 수치를 넘어갈 경우 반환이나 MAX치보다 적어질 경우 오브젝트를 줄 것
*/
template<typename T>
class CustomObjectPool
{
public:
	static CustomObjectPool& instance(int nSize, int maxSize, std::function<T* (T*)> gMethod, std::function<T* (T*)> resetMethod) {
		//C++11에서는 정적 지역 변수 초기화 코드가 멀티스레드 환경에서도 딱 한번 실행
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
					//mutex 영역
					{
						boost::lock_guard<boost::mutex> lock(oMutex);
						//pool에 반환된 오브젝트 존재시
						if (!pool.empty())
						{
							return pool.pop();
						}
						//pool은 비었고 최대치만큼 생성 되지 않았을 때
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
	
	//매개변수 : 기본크기, 최대크기, 오브젝트 세팅 함수 포인터, 오브젝트 초기화 함수 포인터
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