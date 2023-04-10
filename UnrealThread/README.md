# Unreal_Study/Thread
 
### FRunnable 클래스를 상속받아 새로운 클래스를 만든다.

```
class BLUETOOTHWINDOWSPLUGIN_API FBluetoothDataReceiver : public FRunnable
{
}
```

### 생성자 / 소멸자를 만든다.

```
	FBluetoothDataReceiver();
	virtual ~FBluetoothDataReceiver(); //소멸자는 virtual로 만들면 좋다. virtual을 쓰지 않으면 부모 클래스의 소멸자가 호출되서 제대로 된 데이터 회수가 일어나지 않는다.

//생성자
FBluetoothDataReceiver::FBluetoothDataReceiver()
{
    UE_LOG(LogTemp, Warning, TEXT("FBluetoothDataReceiver"));

	m_kill = false;
	m_pause = false;

	//// Create the worker thread
    Thread = MakeShareable(FRunnableThread::Create(this, TEXT("Cadence Thread"), 0, TPri_BelowNormal));

}

//소멸자
FBluetoothDataReceiver::~FBluetoothDataReceiver()
{
    UE_LOG(LogTemp, Warning, TEXT("~FBluetoothDataReceiver"));



	if (Thread.IsValid())
	{

		// Clean up the worker thread
        Thread->Kill();

	}
}

```

### 기본 FRunnable의 Interface 함수를 구현한다.

```
    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
    virtual void Exit() override;

    FThreadSafeBool m_kill;
    FThreadSafeBool m_pause;

bool FBluetoothDataReceiver::Init()
{
    UE_LOG(LogTemp, Warning, TEXT("FBluetoothDataReceiver::Init()"));
	return true;
}

void FBluetoothDataReceiver::Stop()
{
    m_kill = true;
    m_pause = true;

    UE_LOG(LogTemp, Warning, TEXT("FBluetoothDataReceiver::Stop()"));

}

void FBluetoothDataReceiver::Exit()
{
    UE_LOG(LogTemp, Warning, TEXT("FBluetoothDataReceiver::Exit()"));
}

uint32 FBluetoothDataReceiver::Run()
{

while(!m_pause && !m_kill) // 특별한 일이 없는 이상 Run은 계속 실행된다는 의미다.
{

}

return 0;
}

```

### mutex를 만들고, 데이터를 안전하게 읽고 쓰게 해준다.

```
Header

    FCriticalSection m_mutex;

Function A

if (m_mutex.TryLock())
    {
        WheelRevolutions = InWheelRevo;
        WheelEventTimestamp = InWheelTime;
        CrankRevolutions = InCrankRevo;
        CrankEventTimestamp = InCrankTime;

        m_mutex.Unlock();
    }


Function B
    if (m_mutex.TryLock())
    {
        WheelRevo = WheelRevolutions;
        WheelTime = WheelEventTimestamp;
        CrankRevo = CrankRevolutions;
        CrankTime = CrankEventTimestamp;

        m_mutex.Unlock();
    }


```

### 다른 클래스에서 위의 클래스를 생성하여 사용한다.

```
header
 	//스마트 포인터를 사용하면 개발자가 delete 처리를 하지 않아도 안전하게 가비지 컬렉션이 된다.
	TSharedPtr<FBluetoothDataReceiver> DataReceiver;


void ABluetoothDataReader::BeginPlay()
{
	Super::BeginPlay();

	DataReceiver = MakeShareable(new FBluetoothDataReceiver()); //스마트 포인터로 만들기
}

void ABluetoothDataReader::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (DataReceiver.IsValid())
	{
		DataReceiver->EnsureCompletion(); //Stop과 Thread 종료를 하는 함수, 아래 참고
	}

	Super::EndPlay(EndPlayReason);

}

```


### 쓰레드 제어 함수를 만들어 사용하면 더 편하다.

```
    // Thread handling functions
    void EnsureCompletion();        // Function for killing the thread
    void PauseThread();             // Function for pausing the thread
    void ContinueThread();          // Function for continuing/unpausing the thread
    bool IsThreadPaused();          // Function to check the state of the thread

void FBluetoothDataReceiver::PauseThread()
{
	m_pause = true;
}

void FBluetoothDataReceiver::ContinueThread()
{
	m_pause = false;

}

bool FBluetoothDataReceiver::IsThreadPaused()
{
	return (bool)m_pause; //     FThreadSafeBool 라서 (bool)을 써서 캐스팅하고 return 해주어야함.
}

void FBluetoothDataReceiver::EnsureCompletion()
{
    UE_LOG(LogTemp, Warning, TEXT("EnsureCompletion"));

	Stop(); //Stop이 호출되면서     m_kill = true;    m_pause = true;이 되어 Run()이 while()을 그만둔다.

	if (Thread.IsValid())
	{
		Thread->WaitForCompletion();
	}
}




```