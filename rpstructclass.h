struct BAND_DEPENDENCY
{
  enum ZBID zbid;
  enum ZBID rgBandDeps[15];
};

class CScheduleGadgetTransitionsContext
{
public:
	virtual LPVOID GetTransitionContext() = 0;
	virtual LPVOID Invoke(LPVOID transition) = 0;
};