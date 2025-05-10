#include "pch.h" 
#include "CGDraw2DLineLoop.h" 
#include "CG2022112454���έDoc.h"  
//����View֮ǰҪ����Doc 
#include "CG2022112454���έView.h"  //�û��������ƣ�����ͼ�ζ���ͨ��Doc��ӵ����� 
#include "CGLineLoop.h" 

// ���ƶ�ά�� 
static void draw2dline(glm::dvec3& s, glm::dvec3& e)
{
	glBegin(GL_LINES);
	glVertex2d(s.x, s.y);
	glVertex2d(e.x, e.y);
	glEnd();
}

CGDraw2DLineLoop::CGDraw2DLineLoop(GLFWwindow* window):UIEventHandler(window), mStart(0.0, 0.0, 0.0),mPre(0.0,0.0,0.0), mNext(0.0, 0.0, 0.0)
{
}
CGDraw2DLineLoop::~CGDraw2DLineLoop()
{
}

EventType CGDraw2DLineLoop::GetType() //�������� 
{
	//UIEventHandler.h�ж��壬Ҫ���ݲ�ͬ�������������޸ģ��˴��ǻ�պ���
	return EventType::Draw2DLineLoop;
}

//�ٶ�����������ʰȡ�߶���㡢�յ㣬�ƶ����ʱ����Ƥ���� 
int CGDraw2DLineLoop::OnMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (window == nullptr)
		return -1;
	CCG2022112454���έView* view = (CCG2022112454���έView*)glfwGetWindowUserPointer(window);
	if (view == nullptr)
		return -1;
	if (button == GLFW_MOUSE_BUTTON_LEFT) { //����������
		if (action == GLFW_PRESS) {
			mStep++; //ÿ�ε���������ʱ�������������һ 
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			if (mStep == 1) {
				//��һ�ε�������������¼�����λ����Ϊ�������߶ε���� 
				mStart.x = xpos;
				mStart.y = ypos;
				mPoints.push_back({xpos,ypos,0.0});
				mPre = mStart;
				mNext = mStart;
				mPrePos1 = mNext;
				mPrePos2 = mNext;
				view->ShowPrompt("����������������պ��ߵ���һ����");  //ͨ�������¼���View��ʾ��״̬��
			}
			else if (mStep >=1) { //�ڶ��ε�������������¼�����λ����Ϊ��һ����
				if (xpos == mPre.x && ypos == mPre.y) {
					mStep = 1;
					view->ShowPrompt("������غϣ�����������������������һ����");
					return -1;
				}
				//�ж��Ƿ���SHIFT��������ǣ������X��Y�����������С��������ˮƽ�߻�ֱ�� 
				int state = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
				if (state == GLFW_PRESS) {
					if (abs(xpos - mPre.x) <= abs(ypos - mPre.y)) {  //x����仯С����ֱ�� 
						mNext.x = mPre.x; //X���ֲ��� 
						mNext.y = ypos;
					}
					else {  //x����仯��ˮƽ�� 
						mNext.x = xpos;
						mNext.y = mPre.y; //Y���ֲ��� 
					}
				}
				else {  //һ��ֱ�߶Σ���ˮƽ����ֱ������¼�յ㡣 
					mNext.x = xpos;
					mNext.y = ypos;
				}
				mPoints.push_back({ mNext.x,mNext.y,0.0 });
				glm::dvec3 pre = view->DCS2WCS(mPre);
				glm::dvec3 pre2 = view->DCS2WCS(mPrePos2);
				glm::dvec3 next = view->DCS2WCS(mNext);
				//���������Ƥ�� 
				glEnable(GL_COLOR_LOGIC_OP);
				glLogicOp(GL_XOR); // ʹ�����ģʽʵ����Ƥ��Ч��GL_XOR 
				if (mMoveCount == 0) { //��һ���ƶ� 
					(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
					// �������� 
					mPrePos1 = mNext;
					mPrePos2 = mNext;
				}
				else if (mMoveCount == 1) {
					//�ڶ����ƶ� 
					(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
					// �������� 
					mPrePos2 = mPrePos1;
					mPrePos1 = mNext;
				}
				else {
					(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)pre2.x, (int)pre2.y, glm::vec3(0.0f, 0.0f, 1.0f));// �������� 
					(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
					// �������� 
					mPrePos2 = mPrePos1;
					mPrePos1 = mNext;
				}
				glDisable(GL_COLOR_LOGIC_OP);
				glfwSwapBuffers(window);
				//�������ڶ��ε�����ֱ�߶δ�������㡢�յ��ѻ�ȡ�� 

//��������Ҫע������ת������������������Ӧ�����ԡ� 
//��ȡ��㡢�յ��Ӧ�ĳ����������ڴ���ֱ�߶ζ��� 
				(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(1.0f, 1.0f, 1.0f));
				mPre = mNext; //����ǰ����Ϊ��һ��������
				view->ShowPrompt("��������һ����"); //״̬����ʾ 
			}
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			//����Ҽ���������ʼ�պ��߶εĻ���
			if (mStep <= 1 ) {
				view->ShowPrompt("����������������"); //״̬����ʾ 
				return -1;
			}
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			mNext.x = xpos;
			mNext.y = ypos;
			mPoints.push_back({ xpos,ypos,0.0 });//��¼���һ����
			glm::dvec3 pre = view->DCS2WCS(mPre);
			glm::dvec3 pre2 = view->DCS2WCS(mPrePos2);
			glm::dvec3 next = view->DCS2WCS(mNext);
			//���������Ƥ�� 
			glEnable(GL_COLOR_LOGIC_OP);
			glLogicOp(GL_XOR); // ʹ�����ģʽʵ����Ƥ��Ч��GL_XOR 
			glColor3f(0.0f, 0.0f, 1.0f);
			if (mMoveCount == 0) { //��һ���ƶ� 
				(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
				// �������� 
				mPrePos1 = mNext;
				mPrePos2 = mNext;
			}
			else if (mMoveCount == 1) {
				//�ڶ����ƶ� 
				(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
				// �������� 
				mPrePos2 = mPrePos1;
				mPrePos1 = mNext;
			}
			else {
				(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)pre2.x, (int)pre2.y, glm::vec3(0.0f, 0.0f, 1.0f));// �������� 
				(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
				// �������� 
				mPrePos2 = mPrePos1;
				mPrePos1 = mNext;
			}
			glDisable(GL_COLOR_LOGIC_OP);
			glfwSwapBuffers(window);
			std::vector<glm::dvec3> wcsPoints;
			for (const auto& pt : mPoints)
			{
				wcsPoints.push_back(view->DCS2WCS(pt));
			}
			view->AddRenderable(std::make_shared<CGLineLoop>(wcsPoints));//�������ͼ�����뵽������
			view->Invalidate();
			view->UpdateWindow(); //�ͻ���ִ���ػ� 
			mStep = 0;
			mStart.x = mStart.y = mPre.x = mPre.y= mNext.x = mNext.y = 0;
			mPoints.clear();
			view->ShowPrompt("�������µ����"); //״̬����ʾ 
		}

	}

	return 0;
}

int CGDraw2DLineLoop::OnCursorPos(GLFWwindow* window, double xpos, double ypos)
{
	if (window == nullptr)
		return -1;
	CCG2022112454���έView* view = (CCG2022112454���έView*)glfwGetWindowUserPointer(window);
	if (view == nullptr)
		return -1;
	if (mStep == 0) {
		//��û�����һ���㣨��ȡ�߶���㣩 
		view->ShowPrompt("����������������պ��ߵ����");  //״̬����ʾ 

	}
	else if (mStep >= 1) { //�Ѿ����������󣬵�ǰ����ƶ���λ��������γ���Ƥ�� 
		if (xpos == mPre.x && ypos == mPre.y) {
			return -1; //�����غϣ��������� 
		}
		//�ж��Ƿ���SHIFT��������ǣ������X��Y�����������С��������ˮƽ�߻�ֱ�� 
		int state = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
		if (state == GLFW_PRESS) {
			if (abs(xpos - mStart.x) <= abs(ypos - mStart.y)) { //x����仯С����ֱ�� 
				mNext.x = mStart.x;
				mNext.y = ypos;
			}
			else {
				mNext.x = xpos;
				mNext.y = mStart.y;
			}
		}
		else {
			mNext.x = xpos;
			mNext.y = ypos;
		}
		glm::dvec3 pre = view->DCS2WCS(mPre);
		glm::dvec3 pre2 = view->DCS2WCS(mPrePos2);
		glm::dvec3 next = view->DCS2WCS(mNext);
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_XOR); // ʹ�����ģʽʵ����Ƥ��Ч��GL_XOR 
		glColor3f(0.0f, 0.0f, 1.0f);
		if (mMoveCount == 0) { //��һ���ƶ� 
			(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
			// �������� 
			mPrePos1 = mNext;
			mPrePos2 = mNext;
			mMoveCount++;
		}
		else if (mMoveCount == 1) { //�ڶ����ƶ� 
			(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
			mPrePos2 = mPrePos1;
			mPrePos1 = mNext;
			mMoveCount++;
		}
		else {
			(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)pre2.x, (int)pre2.y, glm::vec3(0.0f, 0.0f, 1.0f));// �������� 
			(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
			// �������� 
			mPrePos2 = mPrePos1;
			mPrePos1 = mNext;
			mMoveCount++;
		}
		glDisable(GL_COLOR_LOGIC_OP);
		glfwSwapBuffers(window);
	}
	else {

	}
	return 0;
}

//ȡ������ִ�е�����簴ESC���� 
int CGDraw2DLineLoop::Cancel(GLFWwindow* window)
{
	if (window == nullptr)
		return -1;
	CCG2022112454���έView* view = (CCG2022112454���έView*)glfwGetWindowUserPointer(window);
	if (view == nullptr)
		return -1;
	if (mStep >= 1) { //����Ѿ��������һ�Σ����������ǰɾ����Ƥ�� 
		glm::dvec3 pre = view->DCS2WCS(mPre);
		glm::dvec3 pre2 = view->DCS2WCS(mPrePos2);
		//���������Ƥ�� 
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_XOR); // ʹ�����ģʽʵ����Ƥ��Ч��GL_XOR 
		glColor3f(0.0f, 0.0f, 1.0f);
		draw2dline(pre, pre2);// �������� 
		glDisable(GL_COLOR_LOGIC_OP);
		glfwSwapBuffers(window);

	}
	mStep = 0;
	mStart.x = mStart.y = mPre.x = mPre.y =  mNext.x = mNext.y = 0.0f;
	view->ShowPrompt("����");  //״̬����ʾ 
	return 0;
}