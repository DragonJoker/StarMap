namespace gl
{
	template< typename Type >
	inline Uniform< Type > & UniformBuffer::createUniform
		( std::string const & name )
	{
		Uniform< Type > * ret{ nullptr };
		auto it = m_mapVariables.find( name );

		if ( it == m_mapVariables.end() )
		{
			auto ptr = makeUniform< Type >( name, m_program );

			if ( ptr )
			{
				m_mapVariables.emplace( name, *ptr );
				ret = ptr.get();
				m_listVariables.emplace_back( std::move( ptr ) );
			}
		}
		else
		{
			ret = &static_cast< Uniform< Type > & >( it->second.get() );
		}

		return *ret;
	}
	
	template< typename Type >
	inline ArrayUniform< Type > & UniformBuffer::createUniform
		( std::string const & name
		, int occurences )
	{
		ArrayUniform< Type > * ret{ nullptr };
		auto it = m_mapVariables.find( name );

		if ( it == m_mapVariables.end() )
		{
			auto ptr = makeUniform< Type >( name, occurences, m_program );

			if ( ptr )
			{
				m_mapVariables.emplace( name, *ptr );
				ret = ptr.get();
				m_listVariables.emplace_back( std::move( ptr ) );
			}
		}
		else
		{
			ret = &static_cast< ArrayUniform< Type > & >( it->second.get() );
		}

		return *ret;
	}
}
