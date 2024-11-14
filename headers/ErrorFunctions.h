//
// Created by Eugene Bychkov on 12.11.2024.
//

#ifndef MINIMIZEROPTIMIZER_ERRORFUNCTIONS_H
#define MINIMIZEROPTIMIZER_ERRORFUNCTIONS_H

#include "Function.h"
#include <map>
#include <vector>

/*
    ET_POINTSECTIONDIST,
    ET_POINTONSECTION,
    ET_POINTPOINTDIST,
    ET_POINTONPOINT,
    ET_SECTIONCIRCLEDIST,
    ET_SECTIONONCIRCLE,
    ET_SECTIONINCIRCLE,
    ET_SECTIONSECTIONPARALLEL,
    ET_SECTIONSECTIONPERPENDICULAR,
    ET_SECTIONSECTIONANGEL,
    ET_POINTONCIRCLE
 */


class ErrorFunctions : public Function {
protected:
    Function *c_f;
    std::vector<Variable *> m_X;
public:
    ErrorFunctions(std::vector<Variable *> x) : m_X(x), c_f(nullptr) {}

    std::vector<Variable*> getVariables() {
        return m_X;
    }

    double evaluate() const override{
        return c_f->evaluate();
    }

    Function *derivative(Variable *var) const override{
        return c_f->derivative(var);
    }
    Function* clone() const {
        throw std::runtime_error("Not implemented");
    }
};

//1
class PointSectionDistanceError : public ErrorFunctions {
public:
    PointSectionDistanceError(std::vector<Variable *> x, double error);
};

//2
class PointOnSectionError : public PointSectionDistanceError {
public:
    PointOnSectionError(std::vector<Variable *> x);

};

//3
class PointPointDistanceError : public ErrorFunctions {
public:
    PointPointDistanceError(std::vector<Variable *> x, double error);

};

//4
class PointOnPointError : public PointPointDistanceError {
public:
    PointOnPointError(std::vector<Variable *> x);

};

//5
class SectionCircleDistanceError : public ErrorFunctions {
public:
    SectionCircleDistanceError(std::vector<Variable *> x, double error);

};

//6
class SectionOnCircleError : public SectionCircleDistanceError {
public:
    SectionOnCircleError(std::vector<Variable *> x);

};

//7
class SectionInCircleError : public ErrorFunctions {
public:
    SectionInCircleError(std::vector<Variable *> x);

};

//8
class SectionSectionParallelError : public ErrorFunctions {
public:
    SectionSectionParallelError(std::vector<Variable *> x);

};

//9
class SectionSectionPerpendicularError : public ErrorFunctions {
public:
    SectionSectionPerpendicularError(std::vector<Variable *> x);

};

//10
class SectionSectionAngleError : public ErrorFunctions {
public:
    SectionSectionAngleError(std::vector<Variable *> x, double error);

};

#endif //MINIMIZEROPTIMIZER_ERRORFUNCTIONS_H
