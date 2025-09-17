#include <memory>
class Poligono;
using PoligonoPtr = std::shared_ptr<Poligono>;

#ifndef POLIGONO_H
#define POLIGONO_H

#include "shape.h"

class Poligono : public Shape {
	unsigned int m_vao;
protected:
	Poligono();
public:
	static PoligonoPtr Make();
	virtual ~Poligono();
	virtual void Draw();
};
#endif