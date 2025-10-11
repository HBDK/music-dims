using System.Text.Json.Serialization;
using Microsoft.AspNetCore.Mvc;
using MusicDimsApi.Endpoints.PostPlayMedia.Gateways;

namespace MusicDimsApi.Endpoints.PostPlayMedia;

public static class PostPlayMediaEndpoint
{
    public static WebApplicationBuilder AddPostPlayMediaServices(this WebApplicationBuilder builder)
    {
        var loggerFactory = LoggerFactory.Create(x => x.AddConsole());
        var logger = loggerFactory.CreateLogger(nameof(AddPostPlayMediaServices));
        logger.LogInformation("setting up play media");

        builder.Services.AddScoped<IHaGateway, HaGateway>();
        return builder;
    }

    public static WebApplication MapPostPlayMediaEndpoint(this WebApplication app)
    {
        app.MapPost("/players/{id}/play", async ([FromServices] IHaGateway haGateway, [FromRoute] string id, [FromBody] PostPlayMediaRequestDto dto) =>
        {
            var parts = dto.Link.Split(":");
            var type = parts[1];
            var mediaId = parts.LastOrDefault() ?? throw new Exception("id should have value");
            await haGateway.PlayMedia(id, mediaId, type);
        }).WithName("post-play-media");
 
        return app;
    } 
}

public record PostPlayMediaRequestDto([property: JsonPropertyName("link")]string Link);