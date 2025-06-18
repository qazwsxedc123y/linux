#include "BlockQueue.hpp"

void* Producer(void* arg)
{
	BlockQueue<int>* bq = (BlockQueue<int>*)arg;
	//生产者不断进行生产
	while (true){
		sleep(1);
		int data = rand() % 100 + 1;
		bq->push(data); //生产数据
		std::cout << "Producer: " << data << std::endl;
	}
}
void* Consumer(void* arg)
{
	BlockQueue<int>* bq = (BlockQueue<int>*)arg;
	//消费者不断进行消费
	while (true){
		// sleep(1);
		int data = bq->pop(); //消费数据
		std::cout << "Consumer: " << data << std::endl;
	}
}
int main()
{
	srand((int)time(nullptr));
	pthread_t producer, consumer;
	BlockQueue<int>* bq = new BlockQueue<int>;
	//创建生产者线程和消费者线程
	pthread_create(&producer, nullptr, Producer, bq);
	pthread_create(&consumer, nullptr, Consumer, bq);

	//join生产者线程和消费者线程
	pthread_join(producer, nullptr);
	pthread_join(consumer, nullptr);
	delete bq;
	return 0;
}