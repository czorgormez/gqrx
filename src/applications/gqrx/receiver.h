/* -*- c++ -*- */
/*
 * Gqrx SDR: Software defined radio receiver powered by GNU Radio and Qt
 *           http://gqrx.dk/
 *
 * Copyright 2011-2014 Alexandru Csete OZ9AEC.
 *
 * Gqrx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Gqrx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Gqrx; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
#ifndef RECEIVER_H
#define RECEIVER_H

#include <string>
#include <SoapySDR/Device.hpp>


/**
 * @defgroup DSP Digital signal processing library based on GNU Radio
 */

/**
 * @brief Top-level receiver class.
 * @ingroup DSP
 *
 * This class encapsulates the GNU Radio flow graph for the receiver.
 * Front-ends should only control the receiver through the interface provided
 * by this class.
 */
class receiver
{

public:

    /** Flag used to indicate success or failure of an operation */
    enum status {
        STATUS_OK    = 0, /*!< Operation was successful. */
        STATUS_ERROR = 1  /*!< There was an error. */
    };

    /** Available demodulators. */
    enum rx_demod {
        RX_DEMOD_OFF   = 0,  /*!< No receiver. */
        RX_DEMOD_NONE  = 1,  /*!< No demod. Raw I/Q to audio. */
        RX_DEMOD_AM    = 2,  /*!< Amplitude modulation. */
        RX_DEMOD_NFM   = 3,  /*!< Frequency modulation. */
        RX_DEMOD_WFM_M = 4,  /*!< Frequency modulation (wide, mono). */
        RX_DEMOD_WFM_S = 5,  /*!< Frequency modulation (wide, stereo). */
        RX_DEMOD_WFM_S_OIRT = 6,  /*!< Frequency modulation (wide, stereo oirt). */
        RX_DEMOD_SSB   = 7   /*!< Single Side Band. */
    };

    /** Supported receiver types. */
    enum rx_chain {
        RX_CHAIN_NONE  = 0,   /*!< No receiver, just spectrum analyzer. */
        RX_CHAIN_NBRX  = 1,   /*!< Narrow band receiver (AM, FM, SSB). */
        RX_CHAIN_WFMRX = 2    /*!< Wide band FM receiver (for broadcast). */
    };

    /** Filter shape (convenience wrappers for "transition width"). */
    enum filter_shape {
        FILTER_SHAPE_SOFT = 0,   /*!< Soft: Transition band is TBD of width. */
        FILTER_SHAPE_NORMAL = 1, /*!< Normal: Transition band is TBD of width. */
        FILTER_SHAPE_SHARP = 2   /*!< Sharp: Transition band is TBD of width. */
    };

    receiver(const std::string input_device="",
             const std::string audio_device="",
             unsigned int decimation=1);
    ~receiver();

    void        start();
    void        stop();
    void        set_input_device(const std::string device);
    void        set_output_device(const std::string device);

    std::vector<std::string> get_antennas(void) const;
    void        set_antenna(const std::string &antenna);

    double      set_input_rate(double rate);
    double      get_input_rate(void) const { return d_input_rate; }

    unsigned int    set_input_decim(unsigned int decim);
    unsigned int    get_input_decim(void) const { return d_decim; }

    double      get_quad_rate(void) const {
        return d_input_rate / (double)d_decim;
    }

    double      set_analog_bandwidth(double bw);
    double      get_analog_bandwidth(void) const;

    void        set_iq_swap(bool reversed);
    bool        get_iq_swap(void) const;

    void        set_dc_cancel(bool enable);
    bool        get_dc_cancel(void) const;

    void        set_iq_balance(bool enable);
    bool        get_iq_balance(void) const;

    status      set_rf_freq(double freq_hz);
    double      get_rf_freq(void);
    status      get_rf_range(double *start, double *stop, double *step);

    std::vector<std::string>    get_gain_names();
    status      get_gain_range(std::string &name, double *start, double *stop,
                               double *step) const;
    status      set_auto_gain(bool automatic);
    status      set_gain(std::string name, double value);
    double      get_gain(std::string name) const;

    status      set_filter_offset(double offset_hz);
    double      get_filter_offset(void) const;
    status      set_cw_offset(double offset_hz);
    double      get_cw_offset(void) const;
    status      set_filter(double low, double high, filter_shape shape);
    status      set_freq_corr(double ppm);
    float       get_signal_pwr(bool dbfs) const;
    void        set_iq_fft_size(int newsize);
    void        set_iq_fft_window(int window_type);
    void        get_iq_fft_data(std::complex<float>* fftPoints,
                                unsigned int &fftsize);
    void        get_audio_fft_data(std::complex<float>* fftPoints,
                                   unsigned int &fftsize);

    /* Noise blanker */
    status      set_nb_on(int nbid, bool on);
    status      set_nb_threshold(int nbid, float threshold);

    /* Squelch parameter */
    status      set_sql_level(double level_db);
    status      set_sql_alpha(double alpha);

    /* AGC */
    status      set_agc_on(bool agc_on);
    status      set_agc_hang(bool use_hang);
    status      set_agc_threshold(int threshold);
    status      set_agc_slope(int slope);
    status      set_agc_decay(int decay_ms);
    status      set_agc_manual_gain(int gain);

    status      set_demod(rx_demod demod);

    /* FM parameters */
    status      set_fm_maxdev(float maxdev_hz);
    status      set_fm_deemph(double tau);

    /* AM parameters */
    status      set_am_dcr(bool enabled);

    /* Audio parameters */
    status      set_af_gain(float gain_db);
    status      start_audio_recording(const std::string filename);
    status      stop_audio_recording();
    status      start_audio_playback(const std::string filename);
    status      stop_audio_playback();

    status      start_udp_streaming(const std::string host, int port);
    status      stop_udp_streaming();

    /* I/Q recording and playback */
    status      start_iq_recording(const std::string filename);
    status      stop_iq_recording();
    status      seek_iq_file(long pos);

    /* sample sniffer */
    status      start_sniffer(unsigned int samplrate, int buffsize);
    status      stop_sniffer();
    void        get_sniffer_data(float * outbuff, unsigned int &num);

    bool        is_recording_audio(void) const { return d_recording_wav; }
    bool        is_snifffer_active(void) const { return d_sniffer_active; }

    /* rds functions */
    void        get_rds_data(std::string &outbuff, int &num);
    void        start_rds_decoder(void);
    void        stop_rds_decoder();
    bool        is_rds_decoder_active(void) const;
    void        reset_rds_parser(void);

    SoapySDR::Device *device;
    SoapySDR::Stream *rx_stream;

private:
    void        connect_all(rx_chain type);

private:
    bool        d_running;          /*!< Whether receiver is running or not. */
    double      d_input_rate;       /*!< Input sample rate. */
    double      d_quad_rate;        /*!< Quadrature rate (input_rate / decim) */
    double      d_audio_rate;       /*!< Audio output rate. */
    unsigned int    d_decim;        /*!< input decimation. */
    double      d_rf_freq;          /*!< Current RF frequency. */
    double      d_filter_offset;    /*!< Current filter offset */
    double      d_cw_offset;        /*!< CW offset */
    bool        d_recording_iq;     /*!< Whether we are recording I/Q file. */
    bool        d_recording_wav;    /*!< Whether we are recording WAV file. */
    bool        d_sniffer_active;   /*!< Only one data decoder allowed. */
    bool        d_iq_rev;           /*!< Whether I/Q is reversed or not. */
    bool        d_dc_cancel;        /*!< Enable automatic DC removal. */
    bool        d_iq_balance;       /*!< Enable automatic IQ balance. */

    std::string input_devstr;  /*!< Current input device string. */
    std::string output_devstr; /*!< Current output device string. */

    rx_demod    d_demod;       /*!< Current demodulator. */

    //! Get a path to a file containing random bytes
    static std::string get_random_file(void);

    //! Get a path to a file containing all-zero bytes
    static std::string get_null_file(void);

};

#endif // RECEIVER_H
