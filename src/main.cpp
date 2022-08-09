SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	if (REL::Module::get().version() < SKSE::RUNTIME_SSE_LATEST_AE) {
		ERROR("Unable to load this plugin, incompatible runtime version!\nExpected: Newer than 1-6-317-0 (A.K.A Anniversary Edition)\nDetected: {}", REL::Module::get().version().string());
		return false;
	}

	SKSE::Init(a_skse);

	return true;
}