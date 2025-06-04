// ����������UTF-8
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
    data->rotate = rotate; // ������ת��ΪZ��
    data->range = to; // ������ת��Χ
    data->now = now;
    data->turn = turn;
    std::shared_ptr<RobotBodyRotate> rc = std::make_shared<RobotBodyRotate>();
    node->setUserData(data);
    //���ýڵ���²��� 
    node->SetUpdateCallback(rc);
    //���ýڵ���»ص� 
}

std::shared_ptr<CGTransform> CGRobot::buildRobot()
{
    TessellationHints* hints = tessellationHints();
    bool createBody = (hints ? hints->createBody() : true);
    bool createTop = (hints ? hints->createTop() : true);
    bool createBottom = (hints ? hints->createBottom() : true);
    

    auto robotRoot = std::make_shared<CGTransform>();
    //robotRoot->rotate(20.0f, 0.0f, 1.0f, 0.0f); // ��Y����ת20��
    robotRoot->rotate(-10.0f, 1.0f, 0.0f, 0.0f); // ��X����΢����
    // ���岻ͬ��λ����ɫ
    glm::vec4 colorHead = glm::vec4(0.9f, 0.8f, 0.7f, 1.0f);    // ͷ��: ǳ��ɫ
    glm::vec4 colorChest = glm::vec4(0.8f, 0.2f, 0.2f, 1.0f);   // �ز�: ��ɫ
    glm::vec4 colorAbdomen = glm::vec4(0.2f, 0.2f, 0.8f, 1.0f); // ����: ��ɫ
    glm::vec4 colorPelvis = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);  // �貿: ��ɫ
    glm::vec4 colorUpperArm = glm::vec4(0.2f, 0.7f, 0.2f, 1.0f); // ���: ��ɫ
    glm::vec4 colorLowerArm = glm::vec4(0.1f, 0.5f, 0.1f, 1.0f); // С��: ����ɫ
    glm::vec4 colorHands = glm::vec4(0.9f, 0.8f, 0.7f, 1.0f);   // �ֲ�: ͬͷ����ɫ
    glm::vec4 colorUpperLeg = glm::vec4(0.7f, 0.7f, 0.2f, 1.0f); // ����: ��ɫ
    glm::vec4 colorLowerLeg = glm::vec4(0.5f, 0.5f, 0.1f, 1.0f); // С��: ����ɫ
    glm::vec4 colorFeet = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);    // �Ų�: ���ɫ

    // �������ߴ�
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

    // --- ���ɲ��� --- 
    // �貿 (��Ϊ���ɵĻ���)
    float pelvisY = -abdomenHeight / 2.0f - pelvisHeight / 2.0f;
    auto pelvis = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, pelvisY, 0),
        glm::vec3(torsoWidth-15, pelvisHeight, torsoDepth),
        glm::vec4(0.0f),
        colorPelvis,
        _T("�貿"));
    robotRoot->AddChild(pelvis);

    // ���� (�ڿ貿֮��)
    float abdomenY = pelvisY + pelvisHeight / 2.0f + abdomenHeight / 2.0f;
    auto abdomen = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, abdomenY, 0),
        glm::vec3(torsoWidth * 0.95f, abdomenHeight, torsoDepth * 0.95f), // ������խ
        glm::vec4(0.0f),
        colorAbdomen,
        _T("����"));
    robotRoot->AddChild(abdomen);

    // �ز� (�ڸ���֮��)
    float chestY = abdomenY + abdomenHeight / 2.0f + chestHeight / 2.0f;
    auto chest = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, chestY, 0),
        glm::vec3(torsoWidth, chestHeight, torsoDepth),
        glm::vec4(0.0f),
        colorChest,
        _T("�ز�"));
    robotRoot->AddChild(chest);

    // ͷ�� (���ز�֮��)
    float headY = chestY + chestHeight / 2.0f + headSize / 2.0f;
    auto head = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, headY, 0),
        glm::vec3(headSize, headSize, headSize),
        glm::vec4(0.0f),
        colorHead,
        _T("ͷ��"));
    robotRoot->AddChild(head);

    // --- ��� --- (���ӵ��ز�)
    float shoulderX_L = -torsoWidth / 2.0f - armThickness / 2.0f;
    float shoulderY = chestY; // ��ؽڴ������ز����ĸ߶�

    auto lUpperArmTransform = std::make_shared<CGTransform>();
    lUpperArmTransform->setName("���");
    lUpperArmTransform->translate(shoulderX_L, shoulderY, 0);
    // ��ת: �Ƕ�Ϊ��ֵʱ��ͨ����ʾ��ʱ����ת (��������ת��������ԭ��ʱ)
    lUpperArmTransform->rotate(-75.0f, 0.0f, 0.0f, 1.0f); // ��Z����ת (�ֱ�̧��)
    lUpperArmTransform->rotate(20.0f, 0.0f, 1.0f, 0.0f);  // ��Y����ת (�ֱ���չ)

    auto lUpperArmCube = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, -upperArmLength / 2.0f, 0),
        glm::vec3(armThickness, upperArmLength, armThickness),
        glm::vec4(0.0f),
        colorUpperArm,
        _T("���ϱ�"));
    lUpperArmTransform->AddChild(lUpperArmCube);
    robotRoot->AddChild(lUpperArmTransform);

    auto lLowerArmTransform = std::make_shared<CGTransform>();
    lLowerArmTransform->translate(0, -upperArmLength / 2.0f - lowerArmLength / 2.0f + jointOffset, 0);
    lLowerArmTransform->rotate(-90.0f, 1.0f, 0.0f, 0.0f); // �ⲿ���� (��X��)

    auto lLowerArmCube = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, 0, 0),
        glm::vec3(armThickness * 0.9f, lowerArmLength, armThickness * 0.9f),
        glm::vec4(0.0f),
        colorLowerArm,
        _T("���±�"));
    lLowerArmTransform->AddChild(lLowerArmCube);
    lUpperArmTransform->AddChild(lLowerArmTransform);

    auto lHand = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, -lowerArmLength / 2.0f - handSize / 2.0f + jointOffset / 2.0f, 0),
        glm::vec3(handSize, handSize, handSize),
        glm::vec4(0.0f),
        colorHands,
        _T("����"));
    lLowerArmTransform->AddChild(lHand);

    // --- �ұ� --- (���ӵ��ز�)
    float shoulderX_R = torsoWidth / 2.0f + armThickness / 2.0f;
    auto rUpperArmTransform = std::make_shared<CGTransform>();
    rUpperArmTransform->setName("�ұ�");
    rUpperArmTransform->translate(shoulderX_R, shoulderY, 0);
    rUpperArmTransform->rotate(30.0f, 1.0f, 0.0f, 0.0f);  // ��X����ǰ��ת
    rUpperArmTransform->rotate(-15.0f, 0.0f, 1.0f, 0.0f); // ��Y��������΢��ת

    auto rUpperArmCube = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, -upperArmLength / 2.0f, 0),
        glm::vec3(armThickness, upperArmLength, armThickness),
        glm::vec4(0.0f),
        colorUpperArm,
        _T("���ϱ�"));
    rUpperArmTransform->AddChild(rUpperArmCube);
    robotRoot->AddChild(rUpperArmTransform);

    auto rLowerArmTransform = std::make_shared<CGTransform>();
    rLowerArmTransform->translate(0, -upperArmLength / 2.0f - lowerArmLength / 2.0f + jointOffset, 0);
    rLowerArmTransform->rotate(-45.0f, 1.0f, 0.0f, 0.0f); // �ⲿ����

    auto rLowerArmCube = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, 0, 0),
        glm::vec3(armThickness * 0.9f, lowerArmLength, armThickness * 0.9f),
        glm::vec4(0.0f),
        colorLowerArm,
        _T("���±�"));
    rLowerArmTransform->AddChild(rLowerArmCube);
    rUpperArmTransform->AddChild(rLowerArmTransform);

    auto rHand = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, -lowerArmLength / 2.0f - handSize / 2.0f + jointOffset / 2.0f, 0),
        glm::vec3(handSize, handSize, handSize),
        glm::vec4(0.0f),
        colorHands,
        _T("����"));
    rLowerArmTransform->AddChild(rHand);

    // --- �Ȳ� --- (���ӵ��貿)
    float hipX = torsoWidth / 2.0f - legThickness / 1.5f +10;
    float hipY = pelvisY - pelvisHeight / 2.0f - upperLegLength / 2.0f + jointOffset; // �Źؽ��ڿ貿�·�

    // ����
    auto lUpperLegTransform = std::make_shared<CGTransform>();
	lUpperLegTransform->setName("����");
    lUpperLegTransform->translate(-hipX, hipY, 0);
    // ������ת��ƥ�����ɵĳ�ʼ��ת
    // robotRoot ��Y����ת20�ȣ���X����ת-10��
    // Ϊ�����Ȳ���ǰ��������Ҫ���� robotRoot ��X����ת
    // ��Z�����ת���Ա��֣����ڿ����ȵ��⿪�̶�
    lUpperLegTransform->rotate(-5.0f, 0.0f, 0.0f, 1.0f);  // ��΢����վ�� (��Z��)
    // ������X�����ת��ʹ���Ӿ�����ǰ������ robotRoot �Ѿ�������-10�ȣ�
    // ���ϣ���Ȳ�����ڵ�����ֱ�Ļ�����΢��ǰ���������תֵ��Ҫ��Ӧ������
    // ��������ϣ���Ȳ�������Ѿ����ӵ���������ǰ���5��
    lUpperLegTransform->rotate(5.0f, 1.0f, 0.0f, 0.0f);  // ��΢��ǰ (��X�ᣬ���������ת������)
        
    auto lUpperLegCube = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, 0, 0),
        glm::vec3(legThickness, upperLegLength, legThickness),
        glm::vec4(0.0f),
        colorUpperLeg,
        _T("�����"));
    lUpperLegTransform->AddChild(lUpperLegCube);
    robotRoot->AddChild(lUpperLegTransform);

    auto lLowerLegTransform = std::make_shared<CGTransform>();
    lLowerLegTransform->translate(0, -upperLegLength / 2.0f - lowerLegLength / 2.0f + jointOffset, 0);
    lLowerLegTransform->rotate(-10.0f, 1.0f, 0.0f, 0.0f); // ��΢ϥ������

    auto lLowerLegCube = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, 0, 0),
        glm::vec3(legThickness * 0.9f, lowerLegLength, legThickness * 0.9f),
        glm::vec4(0.0f),
        colorLowerLeg,
        _T("��С��"));
    lLowerLegTransform->AddChild(lLowerLegCube);
    lUpperLegTransform->AddChild(lLowerLegTransform);

    auto lFoot = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, -lowerLegLength / 2.0f - footHeight / 2.0f, footDepth / 2.0f - (legThickness * 0.9f) / 2.0f),
        glm::vec3(footWidth, footHeight, footDepth),
        glm::vec4(0.0f),
        colorFeet,
        _T("���"));
    lLowerLegTransform->AddChild(lFoot);

    // ����
    auto rUpperLegTransform = std::make_shared<CGTransform>();
	rUpperLegTransform->setName("����");
    rUpperLegTransform->translate(hipX, hipY, 0);
    // ������ת��ƥ�����ɵĳ�ʼ��ת
    rUpperLegTransform->rotate(5.0f, 0.0f, 0.0f, 1.0f); // ��΢����վ�� (��Z��)
    // ͬ���ȵ��߼�
    rUpperLegTransform->rotate(5.0f, 1.0f, 0.0f, 0.0f);  // ��΢��ǰ (��X�ᣬ���������ת������)

    auto rUpperLegCube = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, 0, 0),
        glm::vec3(legThickness, upperLegLength, legThickness),
        glm::vec4(0.0f),
        colorUpperLeg,
        _T("�Ҵ���"));
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
        _T("��С��"));
    rLowerLegTransform->AddChild(rLowerLegCube);
    rUpperLegTransform->AddChild(rLowerLegTransform);

    auto rFoot = DrawCube(
        std::make_shared<CGCube>(),
        glm::vec3(0, -lowerLegLength / 2.0f - footHeight / 2.0f, footDepth / 2.0f - (legThickness * 0.9f) / 2.0f),
        glm::vec3(footWidth, footHeight, footDepth),
        glm::vec4(0.0f),
        colorFeet,
        _T("�ҽ�"));
    rLowerLegTransform->AddChild(rFoot);
    moveRobot(rUpperLegTransform, glm::vec3(0, 0, 1), 50, 0, 1);
    moveRobot(lUpperLegTransform, glm::vec3(0, 0, 1), 50, 50, 0);
    moveRobot(rUpperArmTransform, glm::vec3(1, 0, 0), 50, 0, 1);
    moveRobot(robotRoot, glm::vec3(0, 1, 0), 100, 0, 1);
	return robotRoot;
}

