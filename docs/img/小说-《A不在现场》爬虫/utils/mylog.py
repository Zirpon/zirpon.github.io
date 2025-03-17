import logging, coloredlogs

def mylogger(name):
    logger = logging.getLogger(name)

    level_styles = coloredlogs.DEFAULT_LEVEL_STYLES.copy()
    level_styles['debug'] = {'color': 'magenta'}
    level_styles['info'] = {'color': 'yellow'}
    level_styles['error'] = {'color': 'red'}
    level_styles['warning'] = {'color': 'blue'}
    coloredlogs.install(
        level="DEBUG",  # show only debug and above
        #fmt="%(asctime)s - %(hostname)s - %(name)s[%(process)d] -\
        #  %(pathname)s -%(filename)s - %(funcName)s - %(lineno)d - %(module)s - %(levelname)s - %(message)s",
        fmt="%(asctime)s - %(hostname)s - %(name)s[%(process)d] - %(filename)s::%(funcName)s::%(lineno)d - %(levelname)s - %(message)s",
 
        logger=logger,
        level_styles=level_styles,
    )
    return logger

    """
        logger.debug('Print log level：debug')
        logger.info('Print log level：info')
        logger.warning('Print log level：warning')
        logger.error('Print log level：error')
        logger.critical('Print log level：critical')
    """