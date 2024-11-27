# include "CommandCreator.hpp"

template<typename T>
CommandCreatorImpl<T>::CommandCreatorImpl(CommandType type)
					: _type(type) {}

template<typename T>
CommandPtr CommandCreatorImpl<T>::operator()() const {
	return new T(_type);
}

template class CommandCreatorImpl<Command>;
