using Microsoft.AspNetCore.Mvc;
using MusicDimsApi.Endpoints.PostPlayAlbum.Gateways;

namespace MusicDimsApi.Endpoints.PostPlayAlbum;

public static class GetArtistsEndpoint
{
    public static WebApplicationBuilder AddPostPlayAlbumServices(this WebApplicationBuilder builder)
    {
        var loggerFactory = LoggerFactory.Create(x => x.AddConsole());
        var logger = loggerFactory.CreateLogger(nameof(AddPostPlayAlbumServices));
        logger.LogInformation("setting up play album");

        builder.Services.AddScoped<IHaGateway, HaGateway>();
        return builder;
    }

    public static WebApplication MapPostPlayAlbumEndpoint(this WebApplication app)
    {
        app.MapPost("/albums/{id}/play", async ([FromServices] IHaGateway haGateway, [FromRoute] string id) =>
        {
            await haGateway.PlayAlbum(id);
        }).WithName("post-play-album");
 
        return app;
    } 
}