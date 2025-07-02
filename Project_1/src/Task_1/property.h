#ifndef PROPERTY_H_INCLUDED
#define PROPERTY_H_INCLUDED

enum PropertyType {NORMAL, GRASS, FIRE, WATER};

double getRatio(PropertyType attack, PropertyType defense);

#endif // PROPERTY_H_INCLUDED