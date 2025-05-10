#include "pch.h" 
#include "CGDraw2DLineLoop.h" 
#include "CG2022112454冷家苇Doc.h"  
//包含View之前要包含Doc 
#include "CG2022112454冷家苇View.h"  //用户交互绘制，并将图形对象通过Doc添加到场景 
#include "CGLineLoop.h" 

// 绘制二维线 
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

EventType CGDraw2DLineLoop::GetType() //命令类型 
{
	//UIEventHandler.h中定义，要根据不同的命令对象进行修改，此处是绘闭合线
	return EventType::Draw2DLineLoop;
}

//假定鼠标左键单击拾取线段起点、终点，移动鼠标时画橡皮筋线 
int CGDraw2DLineLoop::OnMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (window == nullptr)
		return -1;
	CCG2022112454冷家苇View* view = (CCG2022112454冷家苇View*)glfwGetWindowUserPointer(window);
	if (view == nullptr)
		return -1;
	if (button == GLFW_MOUSE_BUTTON_LEFT) { //鼠标左键单击
		if (action == GLFW_PRESS) {
			mStep++; //每次单击鼠标左键时操作步骤计数加一 
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			if (mStep == 1) {
				//第一次单击鼠标左键，记录鼠标点击位置作为将创建线段的起点 
				mStart.x = xpos;
				mStart.y = ypos;
				mPoints.push_back({xpos,ypos,0.0});
				mPre = mStart;
				mNext = mStart;
				mPrePos1 = mNext;
				mPrePos2 = mNext;
				view->ShowPrompt("请鼠标左键单击输入闭合线的下一个点");  //通过处理事件的View显示到状态栏
			}
			else if (mStep >=1) { //第二次单击鼠标左键，记录鼠标点击位置作为下一个点
				if (xpos == mPre.x && ypos == mPre.y) {
					mStep = 1;
					view->ShowPrompt("与起点重合，请鼠标左键单击重新输入下一个点");
					return -1;
				}
				//判断是否按下SHIFT键，如果是，则根据X、Y方向的增量大小决定绘制水平线或垂直线 
				int state = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
				if (state == GLFW_PRESS) {
					if (abs(xpos - mPre.x) <= abs(ypos - mPre.y)) {  //x方向变化小，垂直线 
						mNext.x = mPre.x; //X保持不变 
						mNext.y = ypos;
					}
					else {  //x方向变化大，水平线 
						mNext.x = xpos;
						mNext.y = mPre.y; //Y保持不变 
					}
				}
				else {  //一般直线段（非水平、垂直），记录终点。 
					mNext.x = xpos;
					mNext.y = ypos;
				}
				mPoints.push_back({ mNext.x,mNext.y,0.0 });
				glm::dvec3 pre = view->DCS2WCS(mPre);
				glm::dvec3 pre2 = view->DCS2WCS(mPrePos2);
				glm::dvec3 next = view->DCS2WCS(mNext);
				//擦除最后橡皮线 
				glEnable(GL_COLOR_LOGIC_OP);
				glLogicOp(GL_XOR); // 使用异或模式实现橡皮线效果GL_XOR 
				if (mMoveCount == 0) { //第一次移动 
					(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
					// 绘制新线 
					mPrePos1 = mNext;
					mPrePos2 = mNext;
				}
				else if (mMoveCount == 1) {
					//第二次移动 
					(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
					// 绘制新线 
					mPrePos2 = mPrePos1;
					mPrePos1 = mNext;
				}
				else {
					(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)pre2.x, (int)pre2.y, glm::vec3(0.0f, 0.0f, 1.0f));// 擦除旧线 
					(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
					// 绘制新线 
					mPrePos2 = mPrePos1;
					mPrePos1 = mNext;
				}
				glDisable(GL_COLOR_LOGIC_OP);
				glfwSwapBuffers(window);
				//鼠标左键第二次单击，直线段创建的起点、终点已获取。 

//创建对象要注意坐标转换到场景，并设置相应的属性。 
//获取起点、终点对应的场景坐标用于创建直线段对象 
				(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(1.0f, 1.0f, 1.0f));
				mPre = mNext; //将当前点作为下一个点的起点
				view->ShowPrompt("请输入下一个点"); //状态栏显示 
			}
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			//鼠标右键单击，开始闭合线段的绘制
			if (mStep <= 1 ) {
				view->ShowPrompt("请输入至少两个点"); //状态栏显示 
				return -1;
			}
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			mNext.x = xpos;
			mNext.y = ypos;
			mPoints.push_back({ xpos,ypos,0.0 });//记录最后一个点
			glm::dvec3 pre = view->DCS2WCS(mPre);
			glm::dvec3 pre2 = view->DCS2WCS(mPrePos2);
			glm::dvec3 next = view->DCS2WCS(mNext);
			//擦除最后橡皮线 
			glEnable(GL_COLOR_LOGIC_OP);
			glLogicOp(GL_XOR); // 使用异或模式实现橡皮线效果GL_XOR 
			glColor3f(0.0f, 0.0f, 1.0f);
			if (mMoveCount == 0) { //第一次移动 
				(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
				// 绘制新线 
				mPrePos1 = mNext;
				mPrePos2 = mNext;
			}
			else if (mMoveCount == 1) {
				//第二次移动 
				(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
				// 绘制新线 
				mPrePos2 = mPrePos1;
				mPrePos1 = mNext;
			}
			else {
				(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)pre2.x, (int)pre2.y, glm::vec3(0.0f, 0.0f, 1.0f));// 擦除旧线 
				(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
				// 绘制新线 
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
			view->AddRenderable(std::make_shared<CGLineLoop>(wcsPoints));//最后将完整图例加入到场景中
			view->Invalidate();
			view->UpdateWindow(); //客户区执行重绘 
			mStep = 0;
			mStart.x = mStart.y = mPre.x = mPre.y= mNext.x = mNext.y = 0;
			mPoints.clear();
			view->ShowPrompt("请输入新的起点"); //状态栏显示 
		}

	}

	return 0;
}

int CGDraw2DLineLoop::OnCursorPos(GLFWwindow* window, double xpos, double ypos)
{
	if (window == nullptr)
		return -1;
	CCG2022112454冷家苇View* view = (CCG2022112454冷家苇View*)glfwGetWindowUserPointer(window);
	if (view == nullptr)
		return -1;
	if (mStep == 0) {
		//还没点击第一个点（获取线段起点） 
		view->ShowPrompt("请鼠标左键单击输入闭合线的起点");  //状态栏显示 

	}
	else if (mStep >= 1) { //已经输入了起点后，当前鼠标移动的位置与起点形成橡皮线 
		if (xpos == mPre.x && ypos == mPre.y) {
			return -1; //两点重合，不做处理 
		}
		//判断是否按下SHIFT键，如果是，则根据X、Y方向的增量大小决定绘制水平线或垂直线 
		int state = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
		if (state == GLFW_PRESS) {
			if (abs(xpos - mStart.x) <= abs(ypos - mStart.y)) { //x方向变化小，垂直线 
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
		glLogicOp(GL_XOR); // 使用异或模式实现橡皮线效果GL_XOR 
		glColor3f(0.0f, 0.0f, 1.0f);
		if (mMoveCount == 0) { //第一次移动 
			(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
			// 绘制新线 
			mPrePos1 = mNext;
			mPrePos2 = mNext;
			mMoveCount++;
		}
		else if (mMoveCount == 1) { //第二次移动 
			(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
			mPrePos2 = mPrePos1;
			mPrePos1 = mNext;
			mMoveCount++;
		}
		else {
			(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)pre2.x, (int)pre2.y, glm::vec3(0.0f, 0.0f, 1.0f));// 擦除旧线 
			(new CCGRenderContext())->MidLine((int)pre.x, (int)pre.y, (int)next.x, (int)next.y, glm::vec3(0.0f, 0.0f, 1.0f));
			// 绘制新线 
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

//取消正在执行的命令（如按ESC键） 
int CGDraw2DLineLoop::Cancel(GLFWwindow* window)
{
	if (window == nullptr)
		return -1;
	CCG2022112454冷家苇View* view = (CCG2022112454冷家苇View*)glfwGetWindowUserPointer(window);
	if (view == nullptr)
		return -1;
	if (mStep >= 1) { //如果已经单击左键一次，则结束操作前删除橡皮线 
		glm::dvec3 pre = view->DCS2WCS(mPre);
		glm::dvec3 pre2 = view->DCS2WCS(mPrePos2);
		//擦除最后橡皮线 
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_XOR); // 使用异或模式实现橡皮线效果GL_XOR 
		glColor3f(0.0f, 0.0f, 1.0f);
		draw2dline(pre, pre2);// 擦除旧线 
		glDisable(GL_COLOR_LOGIC_OP);
		glfwSwapBuffers(window);

	}
	mStep = 0;
	mStart.x = mStart.y = mPre.x = mPre.y =  mNext.x = mNext.y = 0.0f;
	view->ShowPrompt("就绪");  //状态栏显示 
	return 0;
}