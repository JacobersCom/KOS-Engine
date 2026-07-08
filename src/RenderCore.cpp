#include "RenderCore.hpp"
#include "KDevice.hpp"
#include "KSwapchain.hpp"


namespace Kos
{
	namespace 
	{

	}

	void RenderCore::DrawFrame()
	{
		//Waits for all fences to be signed before returning and disables a time out.
		//All fences start off signed so this is oki
		vkWaitForFences(_VkDevice, 1, &m_frames_in_flight, VK_TRUE, UINT64_MAX);
		vkResetFences(_VkDevice, 1, &inFlightFence);

		uint32_t ImageIndex;

		//imageavailablesemaphore signeds when the presentation engine is finish
		vkAcquireNextImageKHR(_VkDevice, _VkSwapchain, UINT64_MAX,
			imageAvailableSemaphore, VK_NULL_HANDLE, &ImageIndex);

		//Ensures the buffer is able to record commands
		vkResetCommandBuffer(_VkCommandBuffer, 0);

		//Record info to submit it
		RecordCommandBuffer(_VkCommandBuffer, ImageIndex);

		VkSubmitInfo SubmittedInfo{};
		SubmittedInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		//The Semaphore upon which to wait on before exe the commandbuffer
		VkSemaphore WaitSemaphore[] = { imageAvailableSemaphore };
		SubmittedInfo.waitSemaphoreCount = 1;
		SubmittedInfo.pWaitSemaphores = WaitSemaphore;

		//Where each semaphore will wait for occur. In this case is when the pipeline writes color
		VkPipelineStageFlags Stage[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		SubmittedInfo.pWaitDstStageMask = Stage;

		SubmittedInfo.commandBufferCount = 1;
		SubmittedInfo.pCommandBuffers = &_VkCommandBuffer;

		//Which semaphore to sign once the commandbuffer is finished
		VkSemaphore SingleSemaphore[] = { renderFinishedSemaphore };
		SubmittedInfo.signalSemaphoreCount = 1;
		SubmittedInfo.pSignalSemaphores = SingleSemaphore;

		//All commands will be submitted to the queue. The fence Will singal when it is finished
		if (vkQueueSubmit(_VkGraphicsQueue, 1, &SubmittedInfo, inFlightFence) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to submit commands to queue");
		}


		VkPresentInfoKHR PresentInfo{};
		PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		//Program waits on these before presenting
		PresentInfo.waitSemaphoreCount = 1;
		PresentInfo.pWaitSemaphores = SingleSemaphore;

		PresentInfo.swapchainCount = 1;
		PresentInfo.pSwapchains = &_VkSwapchain;
		PresentInfo.pImageIndices = &ImageIndex;

		vkQueuePresentKHR(_VkPresentationQueue, &PresentInfo);

		vkDeviceWaitIdle(_VkDevice);

	}
}