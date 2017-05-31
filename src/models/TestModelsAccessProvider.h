#include "ModelsAccessInterface.h"


class TestModelsAccessInterface : public ModelsAccessInterface
{
public:
    TestModelsAccessInterface();
    void createConsumer(const QString &name) override;
    void createShield(const QString &name) override;
    std::vector<Shield> &getAllShields() override;

private:
    std::vector<Shield> shields;
};
