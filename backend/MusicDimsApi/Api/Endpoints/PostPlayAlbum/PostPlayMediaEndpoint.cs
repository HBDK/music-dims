using System.Text.Json.Serialization;
using Microsoft.AspNetCore.Mvc;
using MusicDimsApi.Endpoints.PostPlayAlbum.Gateways;

namespace MusicDimsApi.Endpoints.PostPlayAlbum;

public static class PostPlayAlbumEndpoint
{
    public static WebApplicationBuilder AddPostPlayAlbumServices(this WebApplicationBuilder builder)
    {
        var loggerFactory = LoggerFactory.Create(x => x.AddConsole());
        var logger = loggerFactory.CreateLogger(nameof(AddPostPlayAlbumServices));
        logger.LogInformation("setting up play item");

        builder.Services.AddScoped<IHaGateway, HaGateway>();
        return builder;
    }

    public static WebApplication MapPostPlayAlbumEndpoint(this WebApplication app)
    {
        app.MapPost("/players/{id}/play-album", async ([FromServices] IHaGateway haGateway, [FromRoute] string id, [FromBody] PostPlayAlbumRequestDto dto) =>
        {
            await haGateway.PlayAlbum(id, dto.AlbumId);
        }).WithName("post-play-album");
 
        return app;
    } 
}

public record PostPlayAlbumRequestDto([property: JsonPropertyName("album_id")]string AlbumId);