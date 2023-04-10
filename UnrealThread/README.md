# Unreal_Study/Thread
 
### FRunnable 클래스를 상속받아 새로운 클래스를 만든다.

```
class OBJECTTEST_API FThreadTest : public FRunnable
{
public:
	FThreadTest();

 //소멸자는 virtual로 만들면 좋다. virtual을 쓰지 않으면 부모 클래스의 소멸자가 호출되서 제대로 된 데이터 회수가 일어나지 않는다.
	virtual ~FThreadTest();

protected:

    int32 a = 0;
    float Time;
    TSharedPtr<FRunnableThread> Thread;

    FCriticalSection m_mutex;
    FThreadSafeBool m_kill;
    FThreadSafeBool m_pause;

    // FRunnable interface functions
    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
    virtual void Exit() override;

    void SetData();

public:

    void GetData(int32& A);
};

```

### 생성자 / 소멸자

```
FThreadTest::FThreadTest()
{
	UE_LOG(LogTemp, Warning, TEXT("Thread::Constructor()"));

	Thread = MakeShareable(FRunnableThread::Create(this, TEXT("Thread Name"), 0, TPri_BelowNormal));
	m_kill = false;
	m_pause = false;

	Time = 1.0f;
}


FThreadTest::~FThreadTest()
{
	UE_LOG(LogTemp, Warning, TEXT("Thread::Destructor()"));
	if (Thread.IsValid())
	{
		// Kill을 하지 않아도 스마트 포인터로 묶인 FThreadTest가 가비지 컬렉션 되면서 쓰레드도 같이 가비지 컬렉션이 발동해 시간이 지나면 Stop이 불려서 로직이 종료는 된다. 다만 종료시 랙이 발생하니 Kill해서 빠르게 처리한다.
		Thread->Kill();
	}
}

```

### 기본 FRunnable의 Interface 함수를 구현한다.

```


bool FThreadTest::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Thread::Init()"));

	//false면 실행이 불가능할 것이다.
	return true;
}

uint32 FThreadTest::Run()
{
	UE_LOG(LogTemp, Warning, TEXT("Thread::Run() Begin"));

	while (!m_kill) // kill되면 완전히 빠져나갈 수 있음.
	{
		while (!m_pause) //pause 상태서는 잠시 이 행동만 멈추고 Run의 첫번째 while은 계속 돌 수 있음.
		{
			UE_LOG(LogTemp, Warning, TEXT("Thread::Run() while Loop"));
			SetData();
			FPlatformProcess::Sleep(Time); //Run에 쉬는 시간 없이 계속 돌게하면 컴퓨터 터짐
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Thread::Run() Ended"));

	return 0;
}

void FThreadTest::Stop()
{
	UE_LOG(LogTemp, Warning, TEXT("Thread::Stop()"));

	m_kill = true;
	m_pause = true;

}

void FThreadTest::Exit()
{
	UE_LOG(LogTemp, Warning, TEXT("Thread::Exit()"));

}


```

### mutex를 만들고, 데이터를 안전하게 읽고 쓰게 해준다.

```
void FThreadTest::SetData()
{
	if (m_mutex.TryLock())
	{
		a += 1;

		FPlatformProcess::Sleep(Time); //테스트 용도로 mutex를 받은 상태서 잠시 멈추어 충돌이 나게 만듬.
		m_mutex.Unlock();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetData Failed cause of mutex"));
	}
}

void FThreadTest::GetData(int32& A)
{
	if (m_mutex.TryLock())
	{
		A = a;
		m_mutex.Unlock();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetData Failed cause of mutex"));
	}

}


```

### 다른 클래스에서 위의 클래스를 생성하여 사용한다.

```
header
 	//스마트 포인터를 사용하면 개발자가 delete 처리를 하지 않아도 안전하게 가비지 컬렉션이 된다.
	TSharedPtr<FThreadTest> ThreadTest;


void ASpawningActor::BeginPlay()
{
	Super::BeginPlay();

	ThreadTest = MakeShareable(new FThreadTest); //스마트 포인터에 바인드
}


```

### 호출 순서
```
Constructor()
Init() -> false면 일 안함
Run()
//////Run이 계속 While에 돈다.
//////종료 발동시 아래와 같다.
Destructor()
Stop()
/////Run Ended.
Exit()
```

만약 Init()이 false라면
Run()과 Exit()이 동작하지 않는다.
Destructor() 호출되면 Stop()만 되고 끝난다.