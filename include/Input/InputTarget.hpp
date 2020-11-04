#ifndef MECHA_INPUTTARGET_HPP
#define MECHA_INPUTTARGET_HPP

#include <functional>
#include <unordered_map>
#include <SDL.h>

#include "Input/Action.hpp"

/**
 * Input management inspired by https://github.com/Krozark/SFML-book/blob/master/05_GUI/include/SFML-Book/ActionTarget.hpp
 */

template<typename ActionKey>
class InputTarget {
	public:
		InputTarget<ActionKey>(const InputTarget<ActionKey>&) = delete;
		InputTarget<ActionKey>& operator=(const InputTarget<ActionKey>&) = delete;

		InputTarget<ActionKey>() {}

		bool processEvent(const SDL_Event& event) const;
		void processRealTimeEvents() const;

		using FuncCallback = std::function<void()>;
		void bind(const ActionKey& key, const Action &a, const FuncCallback& callback);

	private:
		std::unordered_map<ActionKey, FuncCallback> cbPoll;
		std::unordered_map<ActionKey, FuncCallback> cbRealTime;

		std::unordered_map<ActionKey, Action> actionMap;
};

#include "Input/InputTarget.tpp"

#endif