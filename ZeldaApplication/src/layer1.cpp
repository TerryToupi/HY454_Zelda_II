#include "layer1.h"

layer1::layer1()
	: Layer("layer1")
{
}

void layer1::onStart()
{
	m_Scene = MakeReference<Scene>(1);
	m_Scene->GetTiles()->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp"); 

	m_linkSheet = MakeReference<AnimationSheet>("link-sheet", "Assets/AnimationFilms/link-sprites.bmp"); 
	m_WalkRight = MakeReference<AnimationFilm>(m_linkSheet.get(), "Assets/Config/Animations/Link/moving_right.json");
	m_WalkLeft = MakeReference<AnimationFilm>(m_linkSheet.get(), "Assets/Config/Animations/Link/moving_Left.json"); 
	m_walkRightAnim = MakeReference<FrameRangeAnimation>("right", 0, m_WalkRight->GetTotalFrames() - 1, 1, 2*16, 2*16, 50);
	m_walkLeftAnim = MakeReference<FrameRangeAnimation>("left", 0, m_WalkLeft->GetTotalFrames() - 1, 1, 2*16, 2*16, 50);

	m_animator1 = MakeReference<FrameRangeAnimator>(m_Scene.get());
	m_animator1.get()->SetOnAction( 
		[this](Animator* animator, const Animation& anim) { return layer1::FrameRangeActionLeft(*this); }
	);
	m_animator2 = MakeReference<FrameRangeAnimator>(m_Scene.get());
	m_animator2.get()->SetOnAction(
		[this](Animator* animator, const Animation& anim) { return layer1::FrameRangeActionRight(*this); }
	);
}

void layer1::onDelete()
{
} 

void layer1::move(Time ts)
{
	float SPEED = 2;

	if (KeyboardInput::IsPressed(SCANCODE_A))
	{
		if (m_Scene->GetTiles()->CanScrollHoriz(-SPEED))
			m_Scene->GetTiles()->Scroll(-SPEED, 0);
	}
	else if (KeyboardInput::IsPressed(SCANCODE_W))
	{
		if (m_Scene->GetTiles()->CanScrollVert(-SPEED))
			m_Scene->GetTiles()->Scroll(0, -SPEED);
	}
	else if (KeyboardInput::IsPressed(SCANCODE_D))
	{
		if (m_Scene->GetTiles()->CanScrollHoriz(+SPEED))
			m_Scene->GetTiles()->Scroll(+SPEED, 0); 
	}
	else if (KeyboardInput::IsPressed(SCANCODE_S))
	{
		if (m_Scene->GetTiles()->CanScrollVert(+SPEED))
			m_Scene->GetTiles()->Scroll(0, +SPEED);
	}
}

void layer1::onUpdate(Time ts)
{ 
	curr = ts;

	move(ts);

	Renderer::BeginScene(m_Scene);
	Renderer::DisplaySceneTiles();
	Renderer::UpdateSceneAnimators(ts);
	Renderer::EndScene();
}

void layer1::onEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyTapEvent>(APP_EVENT_FUNTION(layer1::mover));
	dispatcher.Dispatch<KeyReleaseEvent>(APP_EVENT_FUNTION(layer1::mover));
}

bool layer1::mover(Event& e)
{ 
	if (KeyTapEvent::GetEventTypeStatic() == e.GetEventType())
	{ 
		m_animator2->Start(m_walkRightAnim.get(), curr);
	} 

	if (KeyReleaseEvent::GetEventTypeStatic() == e.GetEventType())
	{ 
		m_animator2->Stop();
	}
	return true;
}

void layer1::FrameRangeActionLeft(layer1& layer)
{
	auto& interbuff = Renderer::InterBufferInstance();
	
	layer.m_WalkLeft->DisplayFrame(
		interbuff,
		{
			layer.m_walkLeftAnim->GetDx(),
			layer.m_walkLeftAnim->GetDy(),
			layer.m_WalkLeft->GetFrameBox(layer.m_animator1->GetCurrFrame()).w,
			layer.m_WalkLeft->GetFrameBox(layer.m_animator1->GetCurrFrame()).h
		},
		layer.m_animator1->GetCurrFrame()
	);
}

void layer1::FrameRangeActionRight(layer1& layer)
{
	auto& interbuff = Renderer::InterBufferInstance();

	layer.m_WalkRight->DisplayFrame(
		interbuff,
		{
			layer.m_walkRightAnim->GetDx(),
			layer.m_walkRightAnim->GetDy(),
			layer.m_WalkRight->GetFrameBox(layer.m_animator2->GetCurrFrame()).w,
			layer.m_WalkRight->GetFrameBox(layer.m_animator2->GetCurrFrame()).h
		},
		layer.m_animator2->GetCurrFrame()
	);
}



