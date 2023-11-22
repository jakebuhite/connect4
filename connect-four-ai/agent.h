#pragma once
class Agent {
public:
    Agent() {}
    virtual ~Agent() {}
    virtual int getAgentMove() = 0;
};