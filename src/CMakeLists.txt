target_sources(${PROJECT_NAME}
	PRIVATE
		main.cpp
		Game.cpp

		MPhysac/MPhysac.cpp
		MPhysac/MPhysacBody.cpp
		MPhysac/MPhysacShape.cpp
		MPhysac/MPhysacWorld.cpp

		Entity/Entity.cpp
		Entity/MovableEntity.cpp
		Entity/PlayerEntity.cpp

		MCamera/MCamera.cpp

		Window.cpp
		Renderer.cpp
		ResourcesLoader.cpp
		Sprite.cpp
		AnimatedSprite.cpp

		Input/Action.cpp
)