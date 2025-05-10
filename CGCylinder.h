#pragma once
#include "CGRenderable.h"

class TessellationHints;

class CGCylinder : public CGRenderable {
    DECLARE_SERIAL(CGCylinder)
public:
    CGCylinder()
        : mRadiusBottom(0.5f), mRadiusTop(0.5f), mHeight(1.0f), mSlices(20), mStacks(1) {
    }

    CGCylinder(float radiusBottom, float radiusTop, float height, int slices, int stacks)
        : mRadiusBottom(radiusBottom), mRadiusTop(radiusTop), mHeight(height),
        mSlices(slices), mStacks(stacks) {
    }

    CGCylinder(const CGCylinder& cyl)
        : mRadiusBottom(cyl.mRadiusBottom), mRadiusTop(cyl.mRadiusTop), mHeight(cyl.mHeight),
        mSlices(cyl.mSlices), mStacks(cyl.mStacks) {
    }

    virtual ~CGCylinder() {}

    void setTessellationHints(std::shared_ptr<TessellationHints> hints);
    TessellationHints* tessellationHints() { return mTessellationHints.get(); }
    const TessellationHints* tessellationHints() const { return mTessellationHints.get(); }

protected:
    virtual void buildDisplayList();

protected:
    float mRadiusBottom;
    float mRadiusTop;
    float mHeight;
    int mSlices;
    int mStacks;
    std::shared_ptr<TessellationHints> mTessellationHints = nullptr;
};
