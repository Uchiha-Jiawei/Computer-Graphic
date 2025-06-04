// 编码声明：UTF-8
#include "pch.h" 
#include "CGRobot.h" 
#include "TessellationHints.h" 
#include "CGCube.h"
#include "CGGeode.h"
#include "CGTransform.h"
#include "RobotBodyTransform.h"
IMPLEMENT_SERIAL(CGRobot, CGRenderable, 1)
CGRobot::CGRobot()
{
}
CGRobot::~CGRobot()
{
}
void CGRobot::setTessellationHints(std::shared_ptr<TessellationHints> hints)

{
    if (mTessellationHints.get() != hints.get())
    {
        mTessellationHints = hints;
        mDisplayListDirty = true;
    }
}

std::shared_ptr<CGTransform> CGRobot::DrawCube(std::shared_ptr<CGCube> cube, glm::vec3 trans, glm::vec3 scale,glm::vec4 rotate,glm::vec4 color,CString name)
{
	auto hint = std::make_shared<TessellationHints>();
	cube->setTessellationHints(hint);
	cube->setDisplayListEnabled(true);
	auto t = std::make_shared<CGTransform>();
	t->setName(name);
	auto c = std::make_shared<CGColor>();
	c->setValue(color); 
	t->gocRenderStateSet()->setRenderState(c, -1);
	t->translate(trans.x, trans.y, trans.z);
    t->rotate(rotate.x,rotate.y, rotate.z, rotate.w);
    t->scale(scale.x, scale.y, scale.z);
	t->AddChild(cube);
	return t;
}

void CGRobot::moveRobot(std::shared_ptr<CGTransform> node, glm::vec3 rotate,float to, float now ,int turn)
{
    std::shared_ptr<RobotBodyTransformParam> data = std::make_shared<RobotBodyTransformParam>();
    data->rotate = rotate; // 设置旋转轴为Z轴
    data->range = to; // 设置旋转范围
    data->now = now;
    data->turn = turn;
    std::shared_ptr<RobotBodyRotate> rc = std::make_shared<RobotBodyRotate>();
    node->setUserData(data);
    //设置节点更新参数 
    node->SetUpdateCallback(rc);
    //设置节点更新回调 
}

std::shared_ptr<CGTransform> CGRobot::buildRobot()
{
    TessellationHints* hints = tessellationHints();
    bool createBody = (hints ? hints->createBody() : true);
    bool createTop = (hints ? hints->createTop() : true);
    bool createBottom = (hints ? hints->createBottom() : true);
    

    auto robotRoot = std::make_shared<CGTransform>();
    //robotRoot->rotate(20.0f, 0.0f, 1.0f, 0.0f); // 绕Y轴旋转20度
    robotRoot->rotate(-10.0f, 1.0f, 0.0f, 0.0f); // 绕X轴轻微俯视
    // 定义不同部位的颜色
    glm::vec4 colorHead = glm::vec4(0.9f, 0.8f, 0.7f, 1.0f);    // 头部: 浅米色
    glm::vec4 colorChest = glm::vec4(0.8f, 0.2f, 0.2f, 1.0f);   // 胸部: 红色
    glm::vec4 colorAbdomen = glm::vec4(0.2f, 0.2f, 0.8f, 1.0f); // 腹部: 蓝色
    glm::vec4 colorPelvis = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);  // 胯部: 灰色
    glm::vec4 colorUpperArm = glm::vec4(0.2f, 0.7f, 0.2f, 1.0f); // 大臂: 绿色
    glm::vec4 colorLowerArm = glm::vec4(0.1f, 0.5f, 0.1f, 1.0f); // 小臂: 深绿色
    glm::vec4 colorHands = glm::vec4(0.9f, 0.8f, 0.7f, 1.0f);   // 手部: 同头部颜色
    glm::vec4 colorUpperLeg = glm::vec4(0.7f, 0.7f, 0.2f, 1.0f); // 大腿: 黄色
    glm::vec4 colorLowerLeg = glm::vec4(0.5f, 0.5f, 0.1f, 1.0f); // 小腿: 暗黄色
    glm::vec4 colorFeet = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);    // 脚部: 深灰色

    // 各部件尺寸
    float totalTorsoHeight = 100.0f;
    float chestHeight = totalTorsoHeight * 0.4f;
    float abdomenHeight = totalTorsoHeight * 0.3f;
    float pelvisHeight = totalTorsoHeight * 0.3f;

    float torsoWidth = 80.0f;
    float torsoDepth = 50.0f;

    float headSize = 40.0f;

    float upperArmLength = 50.0f;
    float lowerArmLength = 44.0f;
    float armThickness = 28.0f;
    float handSize = 20.0f;

    float upperLegLength = 56.0f;
    float lowerLegLength = 50.0f;
    float legThickness = 32.0f;

    float footHeight = 16.0f;
    float footWidth = 34.0f;
    float footDepth = 44.0f;
    float jointOffset = 5.0f;

    // --- 躯干部分 --- 
    // 胯部 (作为躯干的基座)
    float pelvisY = -abdomenHeight / 2.0f - pelvisHeight / 2.0f;
    auto pelvis = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, pelvisY, 0),
        glm::vec3(torsoWidth-15, pelvisHeight, torsoDepth),
        glm::vec4(0.0f),
        colorPelvis,
        _T("胯部"));
    robotRoot->AddChild(pelvis);

    // 腹部 (在胯部之上)
    float abdomenY = pelvisY + pelvisHeight / 2.0f + abdomenHeight / 2.0f;
    auto abdomen = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, abdomenY, 0),
        glm::vec3(torsoWidth * 0.95f, abdomenHeight, torsoDepth * 0.95f), // 腹部略窄
        glm::vec4(0.0f),
        colorAbdomen,
        _T("腹部"));
    robotRoot->AddChild(abdomen);

    // 胸部 (在腹部之上)
    float chestY = abdomenY + abdomenHeight / 2.0f + chestHeight / 2.0f;
    auto chest = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, chestY, 0),
        glm::vec3(torsoWidth, chestHeight, torsoDepth),
        glm::vec4(0.0f),
        colorChest,
        _T("胸部"));
    robotRoot->AddChild(chest);

    // 头部 (在胸部之上)
    float headY = chestY + chestHeight / 2.0f + headSize / 2.0f;
    auto head = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, headY, 0),
        glm::vec3(headSize, headSize, headSize),
        glm::vec4(0.0f),
        colorHead,
        _T("头部"));
    robotRoot->AddChild(head);

    // --- 左臂 --- (连接到胸部)
    float shoulderX_L = -torsoWidth / 2.0f - armThickness / 2.0f;
    float shoulderY = chestY; // 肩关节大致在胸部中心高度

    auto lUpperArmTransform = std::make_shared<CGTransform>();
    lUpperArmTransform->setName("左臂");
    lUpperArmTransform->translate(shoulderX_L, shoulderY, 0);
    // 旋转: 角度为正值时，通常表示逆时针旋转 (当沿着旋转轴正向看向原点时)
    lUpperArmTransform->rotate(-75.0f, 0.0f, 0.0f, 1.0f); // 绕Z轴旋转 (手臂抬起)
    lUpperArmTransform->rotate(20.0f, 0.0f, 1.0f, 0.0f);  // 绕Y轴旋转 (手臂外展)

    auto lUpperArmCube = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, -upperArmLength / 2.0f, 0),
        glm::vec3(armThickness, upperArmLength, armThickness),
        glm::vec4(0.0f),
        colorUpperArm,
        _T("左上臂"));
    lUpperArmTransform->AddChild(lUpperArmCube);
    robotRoot->AddChild(lUpperArmTransform);

    auto lLowerArmTransform = std::make_shared<CGTransform>();
    lLowerArmTransform->translate(0, -upperArmLength / 2.0f - lowerArmLength / 2.0f + jointOffset, 0);
    lLowerArmTransform->rotate(-90.0f, 1.0f, 0.0f, 0.0f); // 肘部弯曲 (绕X轴)

    auto lLowerArmCube = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, 0, 0),
        glm::vec3(armThickness * 0.9f, lowerArmLength, armThickness * 0.9f),
        glm::vec4(0.0f),
        colorLowerArm,
        _T("左下臂"));
    lLowerArmTransform->AddChild(lLowerArmCube);
    lUpperArmTransform->AddChild(lLowerArmTransform);

    auto lHand = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, -lowerArmLength / 2.0f - handSize / 2.0f + jointOffset / 2.0f, 0),
        glm::vec3(handSize, handSize, handSize),
        glm::vec4(0.0f),
        colorHands,
        _T("左手"));
    lLowerArmTransform->AddChild(lHand);

    // --- 右臂 --- (连接到胸部)
    float shoulderX_R = torsoWidth / 2.0f + armThickness / 2.0f;
    auto rUpperArmTransform = std::make_shared<CGTransform>();
    rUpperArmTransform->setName("右臂");
    rUpperArmTransform->translate(shoulderX_R, shoulderY, 0);
    rUpperArmTransform->rotate(30.0f, 1.0f, 0.0f, 0.0f);  // 绕X轴向前旋转
    rUpperArmTransform->rotate(-15.0f, 0.0f, 1.0f, 0.0f); // 绕Y轴向外轻微旋转

    auto rUpperArmCube = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, -upperArmLength / 2.0f, 0),
        glm::vec3(armThickness, upperArmLength, armThickness),
        glm::vec4(0.0f),
        colorUpperArm,
        _T("右上臂"));
    rUpperArmTransform->AddChild(rUpperArmCube);
    robotRoot->AddChild(rUpperArmTransform);

    auto rLowerArmTransform = std::make_shared<CGTransform>();
    rLowerArmTransform->translate(0, -upperArmLength / 2.0f - lowerArmLength / 2.0f + jointOffset, 0);
    rLowerArmTransform->rotate(-45.0f, 1.0f, 0.0f, 0.0f); // 肘部弯曲

    auto rLowerArmCube = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, 0, 0),
        glm::vec3(armThickness * 0.9f, lowerArmLength, armThickness * 0.9f),
        glm::vec4(0.0f),
        colorLowerArm,
        _T("右下臂"));
    rLowerArmTransform->AddChild(rLowerArmCube);
    rUpperArmTransform->AddChild(rLowerArmTransform);

    auto rHand = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, -lowerArmLength / 2.0f - handSize / 2.0f + jointOffset / 2.0f, 0),
        glm::vec3(handSize, handSize, handSize),
        glm::vec4(0.0f),
        colorHands,
        _T("右手"));
    rLowerArmTransform->AddChild(rHand);

    // --- 腿部 --- (连接到胯部)
    float hipX = torsoWidth / 2.0f - legThickness / 1.5f +10;
    float hipY = pelvisY - pelvisHeight / 2.0f - upperLegLength / 2.0f + jointOffset; // 髋关节在胯部下方

    // 左腿
    auto lUpperLegTransform = std::make_shared<CGTransform>();
	lUpperLegTransform->setName("左腿");
    lUpperLegTransform->translate(-hipX, hipY, 0);
    // 调整旋转以匹配躯干的初始旋转
    // robotRoot 绕Y轴旋转20度，绕X轴旋转-10度
    // 为了让腿部向前，我们需要考虑 robotRoot 的X轴旋转
    // 绕Z轴的旋转可以保持，用于控制腿的外开程度
    lUpperLegTransform->rotate(-5.0f, 0.0f, 0.0f, 1.0f);  // 轻微向外站立 (绕Z轴)
    // 调整绕X轴的旋转，使其视觉上向前。由于 robotRoot 已经俯视了-10度，
    // 如果希望腿部相对于地面是直的或者轻微向前，这里的旋转值需要相应调整。
    // 假设我们希望腿部相对于已经俯视的躯干是向前伸出5度
    lUpperLegTransform->rotate(5.0f, 1.0f, 0.0f, 0.0f);  // 轻微向前 (绕X轴，相对于已旋转的躯干)
        
    auto lUpperLegCube = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, 0, 0),
        glm::vec3(legThickness, upperLegLength, legThickness),
        glm::vec4(0.0f),
        colorUpperLeg,
        _T("左大腿"));
    lUpperLegTransform->AddChild(lUpperLegCube);
    robotRoot->AddChild(lUpperLegTransform);

    auto lLowerLegTransform = std::make_shared<CGTransform>();
    lLowerLegTransform->translate(0, -upperLegLength / 2.0f - lowerLegLength / 2.0f + jointOffset, 0);
    lLowerLegTransform->rotate(-10.0f, 1.0f, 0.0f, 0.0f); // 轻微膝盖弯曲

    auto lLowerLegCube = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, 0, 0),
        glm::vec3(legThickness * 0.9f, lowerLegLength, legThickness * 0.9f),
        glm::vec4(0.0f),
        colorLowerLeg,
        _T("左小腿"));
    lLowerLegTransform->AddChild(lLowerLegCube);
    lUpperLegTransform->AddChild(lLowerLegTransform);

    auto lFoot = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, -lowerLegLength / 2.0f - footHeight / 2.0f, footDepth / 2.0f - (legThickness * 0.9f) / 2.0f),
        glm::vec3(footWidth, footHeight, footDepth),
        glm::vec4(0.0f),
        colorFeet,
        _T("左脚"));
    lLowerLegTransform->AddChild(lFoot);

    // 右腿
    auto rUpperLegTransform = std::make_shared<CGTransform>();
	rUpperLegTransform->setName("右腿");
    rUpperLegTransform->translate(hipX, hipY, 0);
    // 调整旋转以匹配躯干的初始旋转
    rUpperLegTransform->rotate(5.0f, 0.0f, 0.0f, 1.0f); // 轻微向外站立 (绕Z轴)
    // 同左腿的逻辑
    rUpperLegTransform->rotate(5.0f, 1.0f, 0.0f, 0.0f);  // 轻微向前 (绕X轴，相对于已旋转的躯干)

    auto rUpperLegCube = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, 0, 0),
        glm::vec3(legThickness, upperLegLength, legThickness),
        glm::vec4(0.0f),
        colorUpperLeg,
        _T("右大腿"));
    rUpperLegTransform->AddChild(rUpperLegCube);
    robotRoot->AddChild(rUpperLegTransform);

    auto rLowerLegTransform = std::make_shared<CGTransform>();
    rLowerLegTransform->translate(0, -upperLegLength / 2.0f - lowerLegLength / 2.0f + jointOffset, 0);
    rLowerLegTransform->rotate(-10.0f, 1.0f, 0.0f, 0.0f);

    auto rLowerLegCube = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, 0, 0),
        glm::vec3(legThickness * 0.9f, lowerLegLength, legThickness * 0.9f),
        glm::vec4(0.0f),
        colorLowerLeg,
        _T("右小腿"));
    rLowerLegTransform->AddChild(rLowerLegCube);
    rUpperLegTransform->AddChild(rLowerLegTransform);

    auto rFoot = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, -lowerLegLength / 2.0f - footHeight / 2.0f, footDepth / 2.0f - (legThickness * 0.9f) / 2.0f),
        glm::vec3(footWidth, footHeight, footDepth),
        glm::vec4(0.0f),
        colorFeet,
        _T("右脚"));
    rLowerLegTransform->AddChild(rFoot);
    moveRobot(rUpperLegTransform, glm::vec3(0, 0, 1), 50, 0, 1);
    moveRobot(lUpperLegTransform, glm::vec3(0, 0, 1), 50, 50, 0);
    moveRobot(rUpperArmTransform, glm::vec3(1, 0, 0), 50, 0, 1);
    moveRobot(robotRoot, glm::vec3(0, 1, 0), 100, 0, 1);
	return robotRoot;
}

