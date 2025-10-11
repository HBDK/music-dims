using Microsoft.AspNetCore.Mvc;
using MusicDimsApi.Endpoints.PostPlayPause.Gateways;

namespace MusicDimsApi.Endpoints.PostPlayPause;

public static class PostPlayPauseEndpoint
{
    public static WebApplicationBuilder AddPostPlayPauseServices(this WebApplicationBuilder builder)
    {
        var loggerFactory = LoggerFactory.Create(x => x.AddConsole());
        var logger = loggerFactory.CreateLogger(nameof(AddPostPlayPauseServices));
        logger.LogInformation("setting up play pause");

        builder.Services.AddScoped<IHaGateway, HaGateway>();
        return builder;
    }

    public static WebApplication MapPostPlayPauseEndpoint(this WebApplication app)
    {
        app.MapPost("/players/{id}/play-pause", async ([FromServices] IHaGateway haGateway, [FromRoute] string id) =>
        {
            await haGateway.PlayPause(id);
        }).WithName("post-play-pause");
 
        return app;
    } 
}