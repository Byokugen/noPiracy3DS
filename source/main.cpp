#include <string>

#include <3ds.h>
#include <sf2d.h>
#include <sfil.h>

#include "topscr_png.h"
#include "botscr_png.h"

using namespace std;

void PTM_ShutdownAsync(void) {
    
    ptmSysmInit();
    
    Handle serviceHandle = 0;
    Result result = srvGetServiceHandle(&serviceHandle, "ptm:sysm");
    if (result != 0) {
        return;
    }
    
    u32 *commandBuffer = getThreadCommandBuffer();
    commandBuffer[0] = 0x040700C0;
    commandBuffer[1] = 0x00000000;
    commandBuffer[2] = 0x00000000;
    commandBuffer[3] = 0x00000000;
    
    svcSendSyncRequest(serviceHandle);
    svcCloseHandle(serviceHandle);
    
    ptmSysmExit();
}

int main() {
    
    
    aptInit();
    sf2d_init();
    
    sf2d_texture* topScreen       = sfil_load_PNG_buffer(topscr_png,        SF2D_PLACE_RAM);
    sf2d_texture* botScreen       = sfil_load_PNG_buffer(botscr_png,        SF2D_PLACE_RAM);
    
    
	// Main loop
	while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        
        
        if (kDown) {
            PTM_ShutdownAsync();break;
        }
        
        
        //Scores
        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
        
        sf2d_draw_texture(botScreen, 0, 0);
        
        sf2d_end_frame();

		// draw instructions
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture(topScreen, 0, 0);
		sf2d_end_frame();

		
		


		sf2d_swapbuffers();
	}
    

    sf2d_free_texture(topScreen);
    sf2d_free_texture(botScreen);

	sf2d_fini();

    
    aptExit();
	return 0;
}
