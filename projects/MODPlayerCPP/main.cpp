
#include <proto/exec.h>

#include <utils/system/system.h>
#include <utils/hardware/custom_registers.h>
#include <utils/devices/mouse.h>
#include <utils/dos/dos.h>
#include <utils/dos/stdio.h>
#include <utils/string/string.h>
#include <utils/sound/ptplayer/ptplayer.h>

static void ShowInfo(void)
{
	char buffer[200];

	FormatString(buffer, sizeof(buffer), "Press left mousebutton to start...");
	PrintString(buffer);

	while(!CheckLeftMouseRelease());

	FormatString(buffer, sizeof(buffer), "\n\rPress left mousebutton to exit...\n\r");
	PrintString(buffer);
}

static bool playMOD(const char *modName)
{
	uint32_t size = 0;
	char *modFile = LoadFile(modName, MEMF_CHIP, &size);

	if (modFile)
	{
		char *samples = nullptr;
		uint32_t position = 0;
		void *hwregbase = (void *)HWREGBASE;

		mt_install_cia(hwregbase, nullptr, PAL_SYSTEM);
		mt_init(hwregbase, modFile, samples, position);
		mt_Enable = true;

		while(!CheckLeftMouseRelease());

		mt_end(hwregbase);
		mt_remove_cia(hwregbase);

		FreeMem(modFile, size);

		return true;
	}

	return false;
}

STDARGS int main(int argc, char *argv[])
{
	OpenDOSLibrary();
	ShowInfo();

	SystemSave();
	EnableDiskIO();

	const char *modName = "DemoTrack.mod";
	bool success = playMOD(modName);

	SystemRestore();

	if (!success)
	{
		char buffer[200];

		FormatString(buffer, sizeof(buffer), "Unable to load %s\n\r", modName);
		PrintString(buffer);
	}

	CloseDOSLibrary();

	return 0;
}
