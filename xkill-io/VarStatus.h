#ifndef VARSTATUS_H
#define VARSTATUS_H

template <class Var>
class VarStatus
{
private:
	Var*	var_;
	bool	status_;
protected:
public:
	VarStatus(Var* var, bool status)
	{
		this->var_		= var;
		this->status_	= status;
	}
	VarStatus(bool status)
	{
		this->status_	= status;
		var_			= nullptr;
	}
	VarStatus()
	{
		status_	= false;
		var_	= nullptr;
	}
	~VarStatus()
	{
		if(status_)
		{
			if(var_)
				delete var_;
		}
	}

	bool operator==(const bool &status) const 
	{
		bool res;
		if(status_ == status)
			res = true;
		else
			res = false;
		return res;
	}
	void operator=(const bool &status) 
	{
		status_ = status;
	}

	Var* getVar()		const { return var_;		}
	bool getStatus()	const { return status_;		}

	void setStatus(bool status) { status_	= status;	}
	void setVar(Var* var)		{ var_		= var;		}
};

#endif //VARSTATUS_H