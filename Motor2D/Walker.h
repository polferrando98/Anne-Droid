
#include "j1Entity.h"
class Walker :
	public Entity
{
public:
	Walker(fPoint position);
	~Walker();

	bool Update(float dt);


};
