#pragma once
#include "boost/asio.hpp"
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
	static CustomObjectPool<T>* instance(int nSize, int maxSize, std::function <boost::shared_ptr<T>(boost::shared_ptr<T>)> gMethod,
		std::function <boost::shared_ptr<T>(boost::shared_ptr<T>)> resetMethod) {
		//C++11������ ���� ���� ���� �ʱ�ȭ �ڵ尡 ��Ƽ������ ȯ�濡���� �� �ѹ� ����
		//thread-safe

		static CustomObjectPool<T>* instance = new CustomObjectPool<T>(nSize, maxSize, gMethod, resetMethod);

		return instance;
	}

	boost::shared_ptr<T> getObject()
	{
		if (pool.empty())
		{
			if (existObjectCnt < mMaxSize)
			{
				{
					boost::lock_guard<boost::mutex> lock(oMutex);
					existObjectCnt++;
				}
				return mGMethod(boost::make_shared<T>());
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
							return mGMethod(boost::make_shared<T>());
						}

					}
					boost::this_thread::sleep_for(boost::chrono::milliseconds(30));

				}

			}
		}
		else {
			boost::lock_guard<boost::mutex> lock(oMutex);
			return pool.pop();
		}

	}

	void returnObject(boost::shared_ptr<T> object)
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
	CustomObjectPool(int nSize, int maxSize, std::function <boost::shared_ptr<T>(boost::shared_ptr<T>)> gMethod,
		std::function <boost::shared_ptr<T>(boost::shared_ptr<T>)> resetMethod)
		:mSize(nSize), mMaxSize(maxSize), mResetMethod(resetMethod), mGMethod(gMethod)
	{

		for (int i = 0; i < mSize; i++)
		{
			pool.push(mGMethod(boost::make_shared<T>()));
		}
		std::cout << "complete";
	}

	std::queue<boost::shared_ptr<T>> pool;
	int mSize, mMaxSize;
	int existObjectCnt;
	boost::mutex oMutex;
	std::function <boost::shared_ptr<T>(boost::shared_ptr<T>)> mGMethod;
	std::function <boost::shared_ptr<T>(boost::shared_ptr<T>)> mResetMethod;

};