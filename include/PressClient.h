#include "Socket.h"
#include "Client.h"
#include "Semaphore.h"
#include "Mutex.h"


#include <stdio.h>
#include <memory>
#include <string.h>


namespace app {
class PressClient;

class PressHandleError : public net::HandleError {
public:
	typedef std::shared_ptr<PressHandleError> ptr;
	PressHandleError(PressClient* pc) { m_pc = pc; }
protected:
	virtual void handleError() override;

private:
	PressClient* m_pc;

}; // class PressHandleError

class PressClient : public Client {
public:
	typedef std::shared_ptr<PressClient> ptr;
	PressClient(const char* addr);
	~PressClient();

	virtual bool connect(const char* addr) override;
	virtual bool close() override;

	virtual bool set() override;
	virtual bool get() override;
	virtual bool wait() override;

	void notice();
	void setPress(char press[3]) { 
		thread::Mutex mutex;
		memcpy(m_setPressVal, press, 3); 
	}

	bool reconnect(int n_times = 3);

public:
	static const char* GetPress() { 
		thread::Mutex mutex;
		const char* rt = s_rtPress; 
		return rt;
	}

	static void SetPress(const char* press) { 
		thread::Mutex mutex;
		memcpy(s_rtPress, press, 3); 
	
	}	

private:
	net::Path::ptr m_path;
	net::Socket::ptr m_sock;
	thread::Semaphore::ptr m_sem;

	char		m_setPressVal[3];

	PressHandleError::ptr m_handleError;
	static char s_rtPress[3];
	static const char s_cmdGet[5];
	static const char s_cmdSet[5];
}; // PressClient;
const char PressClient::s_cmdSet[5] = { 0x53, 0x4F, 0x55, 0x52, 0x20 };
const char PressClient::s_cmdGet[5] = { 0x53, 0x4F, 0x55, 0x52, 0x4F };
char PressClient::s_rtPress[3] = { 0 };
} // namespace app




















































