target_sources(${PROJECT_NAME}
	PRIVATE
		main.cpp
		Game.cpp

		MPhysac/MPhysac.cpp
		MPhysac/MPhysacBody.cpp
		MPhysac/MPhysacShape.cpp
		MPhysac/MPhysacWorld.cpp

		Entity/Entity.cpp
		Entity/PlayerEntity.cpp

		MCamera/MCamera.cpp
)