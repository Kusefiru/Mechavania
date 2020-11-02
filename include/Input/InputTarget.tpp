template<typename ActionKey>
bool InputTarget<ActionKey>::processEvent(const SDL_Event& event) const {
	if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP)
		return false;
	
	bool res = false;

	for (auto cb : cbPoll) {
		const Action &act = actionMap.at(cb.first);
		if (act == event) {
			cb.second();
			res = true;
		}
	}

	return res;
}

template<typename ActionKey>
void InputTarget<ActionKey>::processRealTimeEvents() const {
	const Uint8 *kbState = SDL_GetKeyboardState(NULL);

	for (auto cb : cbRealTime) {
		if (actionMap.at(cb.first).test(kbState)) {
			cb.second();
		}
	}
}

template<typename ActionKey>
void InputTarget<ActionKey>::bind(const ActionKey& key, const Action &a, const FuncCallback& callback) {
	if (a.type & Action::Type::RealTime)
		cbRealTime.insert(std::make_pair(key, callback));
	else
		cbPoll.insert(std::make_pair(key, callback));
	
	actionMap.insert(std::make_pair(key, a));
}