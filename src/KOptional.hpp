
namespace Kos
{
	template<typename T>
	class KOptional
	{
	public:
		KOptional() { m_value = nullptr; };
		KOptional(T value) : m_value(value), has_value(false){};

		bool HasValue() { return &m_value != nullptr; };

	private:
		T* m_value;
		bool has_value;
	};
}