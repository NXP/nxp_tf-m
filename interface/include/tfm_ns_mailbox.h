/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* Data types and API definitions in NSPE mailbox library */

#ifndef __TFM_NS_MAILBOX_H__
#define __TFM_NS_MAILBOX_H__

#include <stdbool.h>
#include <stdint.h>
#include "tfm_mailbox.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TFM_MULTI_CORE_TEST
/**
 * \brief The structure to hold the statistics result of NSPE mailbox
 */
struct ns_mailbox_stats_res_t {
    uint8_t avg_nr_slots;               /* The value before the decimal point
                                         * in the average number of NSPE
                                         * mailbox slots in use.
                                         */
    uint8_t avg_nr_slots_tenths;        /* The first digit value after the
                                         * decimal point in the average
                                         * number of NSPE mailbox slots in use.
                                         */
};
#endif

/**
 * \brief NSPE mailbox initialization
 *
 * \param[in] queue             The base address of NSPE mailbox queue to be
 *                              initialized.
 *
 * \retval MAILBOX_SUCCESS      Operation succeeded.
 * \retval Other return code    Operation failed with an error code.
 */
int32_t tfm_ns_mailbox_init(struct ns_mailbox_queue_t *queue);

/**
 * \brief Send PSA client call to SPE via mailbox. Wait and fetch PSA client
 *        call result.
 *
 * \param[in] call_type         PSA client call type
 * \param[in] params            Parameters used for PSA client call
 * \param[in] client_id         Optional client ID of non-secure caller.
 *                              It is required to identify the non-secure caller
 *                              when NSPE OS enforces non-secure task isolation.
 * \param[out] reply            The buffer written with PSA client call result.
 *
 * \retval MAILBOX_SUCCESS      The PSA client call is completed successfully.
 * \retval Other return code    Operation failed with an error code.
 */
int32_t tfm_ns_mailbox_client_call(uint32_t call_type,
                                   const struct psa_client_params_t *params,
                                   int32_t client_id,
                                   int32_t *reply);

/**
 * \brief Wake up the owner task of the first replied mailbox message in the
 *        NSPE mailbox queue.
 *        This function is intended to be called inside platform specific
 *        notification IRQ handler.
 *
 * \note The replied status of the fetched mailbox message will be cleaned after
 *       the message is fetched. When this function is called again, it wakes
 *       the owner task of next replied mailbox message from the NSPE mailbox
 *       queue.
 *
 * \return MAILBOX_SUCCESS       The task of the first replied mailbox message
 *                               is found and wake-up signal is sent.
 * \return MAILBOX_NO_PEND_EVENT No replied mailbox message is found.
 * \return Other return code     Failed with an error code
 */
int32_t tfm_ns_mailbox_wake_reply_owner_isr(void);

/**
 * \brief Platform specific NSPE mailbox initialization.
 *        Invoked by \ref tfm_ns_mailbox_init().
 *
 * \param[in] queue             The base address of NSPE mailbox queue to be
 *                              initialized.
 *
 * \retval MAILBOX_SUCCESS      Operation succeeded.
 * \retval Other return code    Operation failed with an error code.
 */
int32_t tfm_ns_mailbox_hal_init(struct ns_mailbox_queue_t *queue);

/**
 * \brief Notify SPE to deal with the PSA client call sent via mailbox
 *
 * \note The implementation depends on platform specific hardware and use case.
 *
 * \retval MAILBOX_SUCCESS      Operation succeeded.
 * \retval Other return code    Operation failed with an error code.
 */
int32_t tfm_ns_mailbox_hal_notify_peer(void);

/**
 * \brief Enter critical section of NSPE mailbox.
 *
 * \note The implementation depends on platform specific hardware and use case.
 */
void tfm_ns_mailbox_hal_enter_critical(void);

/**
 * \brief Exit critical section of NSPE mailbox.
 *
 * \note The implementation depends on platform specific hardware and use case.
 */
void tfm_ns_mailbox_hal_exit_critical(void);

/**
 * \brief Enter critical section of NSPE mailbox in IRQ handler.
 *
 * \note The implementation depends on platform specific hardware and use case.
 */
void tfm_ns_mailbox_hal_enter_critical_isr(void);

/**
 * \brief Enter critical section of NSPE mailbox in IRQ handler
 *
 * \note The implementation depends on platform specific hardware and use case.
 */
void tfm_ns_mailbox_hal_exit_critical_isr(void);

#ifdef FORWARD_PROT_MSG
static inline int32_t tfm_ns_mailbox_os_lock_init(void)
{
    return MAILBOX_SUCCESS;
}

static inline uint32_t tfm_ns_mailbox_os_lock_acquire(void)
{
    return MAILBOX_SUCCESS;
}

static inline uint32_t tfm_ns_mailbox_os_lock_release(void)
{
    return MAILBOX_SUCCESS;
}
#else /* FORWARD_PROT_MSG */
/**
 * \brief Initialize the multi-core lock for synchronizing PSA client call(s)
 *        The actual implementation depends on the non-secure use scenario.
 *
 * \return \ref MAILBOX_SUCCESS on success
 * \return \ref MAILBOX_GENERIC_ERROR on error
 */
int32_t tfm_ns_mailbox_os_lock_init(void);

/**
 * \brief Acquire the multi-core lock for synchronizing PSA client call(s)
 *        The actual implementation depends on the non-secure use scenario.
 *
 * \return \ref MAILBOX_SUCCESS on success
 * \return \ref MAILBOX_GENERIC_ERROR on error
 */
int32_t tfm_ns_mailbox_os_lock_acquire(void);

/**
 * \brief Release the multi-core lock for synchronizing PSA client call(s)
 *        The actual implementation depends on the non-secure use scenario.
 *
 * \return \ref MAILBOX_SUCCESS on success
 * \return \ref MAILBOX_GENERIC_ERROR on error
 */
int32_t tfm_ns_mailbox_os_lock_release(void);
#endif /* FORWARD_PROT_MSG */

#ifdef TFM_MULTI_CORE_MULTI_CLIENT_CALL
/**
 * \brief Get the handle of the current non-secure task executing mailbox
 *        functionalities
 *
 * \note This function should be implemented according to NS OS and
 *       actual use scenario.
 *       This function can be ignored or return NULL if sleep/wake-up mechanism
 *       is not required in PSA Client API implementation.
 *
 * \return Return the handle of task.
 */
const void *tfm_ns_mailbox_os_get_task_handle(void);

/**
 * \brief Performs use scenario and NS OS specific waiting mechanism to wait for
 *        the reply of the specified mailbox message to be returned from SPE.
 *
 * \note This function is implemented by NS OS specific waiting mechanism
 *       according to use scenario.
 *
 * \param[in] handle            The handle of mailbox message.
 */
void tfm_ns_mailbox_os_wait_reply(mailbox_msg_handle_t handle);

/*
 * \brief Performs use scenario and NS OS specific mechanism in a mailbox IRQ
 *        handler, to wake up a sleeping task which is waiting for its mailbox
 *        message reply.
 *
 * \note The underlying NS OS specific function called inside this function
 *       should be able to work in an IRQ handler.
 *
 * \note This function is implemented by NS OS specific waiting
 *       mechanism according to use scenario.
 *
 * \param[in] task_handle       The handle to the task to be woken up.
 * \param[in] handle            The mailbox handle which can be used as thread
 *                              flag.
 */
void tfm_ns_mailbox_os_wake_task_isr(const void *task_handle,
                                     mailbox_msg_handle_t handle);
#else /* TFM_MULTI_CORE_MULTI_CLIENT_CALL */
#define tfm_ns_mailbox_os_wait_reply(handle)        do {} while (0)

static inline const void *tfm_ns_mailbox_os_get_task_handle(void)
{
    return NULL;
}

#define tfm_ns_mailbox_os_wake_task_isr(task, handle)    do {} while (0)
#endif /* TFM_MULTI_CORE_MULTI_CLIENT_CALL */

#ifdef TFM_MULTI_CORE_TEST
/**
 * \brief Initialize the statistics module in TF-M NSPE mailbox.
 *
 * \note This function is only available when multi-core tests are enabled.
 */
void tfm_ns_mailbox_tx_stats_init(void);

/**
 * \brief Calculate the average number of used NS mailbox queue slots each time
 *        NS task requires a queue slot to submit mailbox message, which is
 *        recorded in NS mailbox statisitics module.
 *
 * \note This function is only available when multi-core tests are enabled.
 *
 * \param[in] stats_res         The buffer to be written with
 *                              \ref ns_mailbox_stats_res_t.
 *
 * \return Return the calculation result.
 */
void tfm_ns_mailbox_stats_avg_slot(struct ns_mailbox_stats_res_t *stats_res);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __TFM_NS_MAILBOX_H__ */
