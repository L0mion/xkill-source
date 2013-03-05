#ifndef TIMERDX_H
#define TIMERDX_H

struct ID3D11Query;
struct ID3D11Device;
struct ID3D11DeviceContext;

class TimerDX
{
private:
	ID3D11Query*	start;
	ID3D11Query*	stop;
	ID3D11Query*	disjoint;
protected:
public:
	void init(ID3D11Device* device);

	void startTimer(ID3D11DeviceContext*	deviceContext);
	void stopTimer(ID3D11DeviceContext*		deviceContext);

	double Time(ID3D11DeviceContext* deviceContext);

	TimerDX();
	~TimerDX();
};

#endif //TIMERDX_H