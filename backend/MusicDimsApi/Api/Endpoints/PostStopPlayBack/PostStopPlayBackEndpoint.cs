using Microsoft.AspNetCore.Mvc;
using MusicDimsApi.Endpoints.PostStopPlayBack.Gateways;

namespace MusicDimsApi.Endpoints.PostStopPlayBack;

public static class PostStopPlayBackEndpoint
{
    public static WebApplicationBuilder AddPostStopPlayBackServices(this WebApplicationBuilder builder)
    {
        var loggerFactory = LoggerFactory.Create(x => x.AddConsole());
        var logger = loggerFactory.CreateLogger(nameof(AddPostStopPlayBackServices));
        logger.LogInformation("setting up stop playback");

        builder.Services.AddScoped<IHaGateway, HaGateway>();
        return builder;
    }

    public static WebApplication MapPostStopPlayBackEndpoint(this WebApplication app)
    {
        app.MapPost("/players/{id}/stop", async ([FromServices] IHaGateway haGateway, [FromRoute] string id) =>
        {
            await haGateway.StopPlayback(id);
        }).WithName("post-stop-playback");
 
        return app;
    } 
}