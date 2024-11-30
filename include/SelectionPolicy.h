#pragma once
#include <vector>
#include <string>
using std::vector;
using std::string;

class FacilityType;

class SelectionPolicy {
    public:
        virtual ~SelectionPolicy() = default;
        virtual SelectionPolicy* clone() const = 0;
        virtual const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) = 0;
        virtual const string toString() const = 0;
};

class NaiveSelection : public SelectionPolicy {
    public:
        NaiveSelection();
        SelectionPolicy* clone() const override;
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;

    private:
        int lastSelectedIndex;
};

class BalancedSelection : public SelectionPolicy {
    public:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
        SelectionPolicy* clone() const override;
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;

    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;
};

class EconomySelection : public SelectionPolicy {
    public:
        EconomySelection();
        SelectionPolicy* clone() const override;
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;

    private:
        int lastSelectedIndex;
};

class SustainabilitySelection : public SelectionPolicy {
    public:
        SustainabilitySelection();
        SelectionPolicy* clone() const override;
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;

    private:
        int lastSelectedIndex;
};